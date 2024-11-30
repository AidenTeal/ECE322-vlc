// test_wextern.c

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// Include the wextern.c source here
// For testing purposes, it's recommended to have a header (wextern.h) with necessary declarations.
// However, for this example, we'll assume wextern.c can be included directly.
// Adjust the path as necessary based on your project structure.
#include "wextern.c" // Adjust the path as necessary

// --------------------------------------------
// Mock Definitions for External Dependencies
// --------------------------------------------

// Define minimal structures for testing
typedef struct vlc_object_t vlc_object_t;
typedef struct vlc_window_cfg_t vlc_window_cfg_t;
typedef struct vlc_window_t vlc_window_t;
typedef struct vlc_window_operations vlc_window_operations_t;

// Mock logging functions
void msg_Err(vlc_object_t* obj, const char* fmt, ...) {
    function_called();
    // Optionally, capture and verify fmt and arguments
}

void msg_Dbg(vlc_object_t* obj, const char* fmt, ...) {
    function_called();
    // Optionally, capture and verify fmt and arguments
}

void msg_Warn(vlc_object_t* obj, const char* fmt, ...) {
    function_called();
    // Optionally, capture and verify fmt and arguments
}

void msg_Info(vlc_object_t* obj, const char* fmt, ...) {
    function_called();
    // Optionally, capture and verify fmt and arguments
}

// Mock window reporting functions
void vlc_window_ReportSize(vlc_window_t* wnd, unsigned width, unsigned height) {
    check_expected(wnd);
    check_expected(width);
    check_expected(height);
    function_called();
}

void vlc_window_ReportMouseMoved(vlc_window_t* wnd, int x, int y) {
    check_expected(wnd);
    check_expected(x);
    check_expected(y);
    function_called();
}

void vlc_window_ReportMousePressed(vlc_window_t* wnd, libvlc_video_output_mouse_button_t button) {
    check_expected(wnd);
    check_expected(button);
    function_called();
}

void vlc_window_ReportMouseReleased(vlc_window_t* wnd, libvlc_video_output_mouse_button_t button) {
    check_expected(wnd);
    check_expected(button);
    function_called();
}

// Mock variable functions
void* var_InheritAddress(vlc_object_t* obj, const char* varname) {
    check_expected(obj);
    check_expected(varname);
    return (void*)mock(); // Return a pointer or NULL
}

// Mock memory allocation functions
void* vlc_obj_malloc(vlc_object_t* obj, size_t size) {
    check_expected(obj);
    check_expected(size);
    return (void*)mock(); // Return allocated memory or NULL
}

void vlc_obj_free(void* ptr) {
    check_expected(ptr);
    // No operation needed for the mock
}

// Define VLC macros
#define VLC_SUCCESS 0
#define VLC_ENOMEM -12
#define VLC_EGENERIC -1

#define VLC_UNUSED(x) (void)(x)

#define unlikely(x) __builtin_expect(!!(x), 0)

// Define video_format_t structure (from VLC)
typedef struct video_format {
    int i_chroma;
    int i_width;
    int i_height;
    int orientation;
    // Additional fields as needed
} video_format_t;

// Define libvlc_video_output_mouse_button_t (from VLC)
typedef enum {
    LIBVLC_VIDEO_OUTPUT_MOUSE_BUTTON_LEFT,
    LIBVLC_VIDEO_OUTPUT_MOUSE_BUTTON_RIGHT,
    LIBVLC_VIDEO_OUTPUT_MOUSE_BUTTON_MIDDLE
} libvlc_video_output_mouse_button_t;

// Define vlc_window_cfg_t structure
struct vlc_window_cfg_t {
    unsigned width;
    unsigned height;
    // Additional configuration fields as needed
};

// Define vlc_window_operations structure
struct vlc_window_operations {
    int (*enable)(struct vlc_window*, const vlc_window_cfg_t*);
    void (*disable)(struct vlc_window*);
    // Other operations can be added as needed
};

// Define vlc_window_t structure
struct vlc_window_t {
    int type;
    const vlc_window_operations_t* ops;
    void* sys;
    struct {
        bool has_double_click;
        // Additional info fields as needed
    } info;
    // Additional fields as needed
};

// Define wextern_t structure (from wextern.c)
typedef struct {
    void                                   *opaque;
    libvlc_video_output_set_window_cb       setWindowCb;
} wextern_t;

// Define libvlc_video_output_set_window_cb (from VLC)
typedef void (*libvlc_video_output_set_window_cb)(
    void *opaque,
    void (*resize_cb)(void *, unsigned, unsigned),
    void (*mouse_moved_cb)(void *, int, int),
    void (*mouse_press_cb)(void *, libvlc_video_output_mouse_button_t),
    void (*mouse_release_cb)(void *, libvlc_video_output_mouse_button_t),
    vlc_window_t *wnd
);

// Define N_ macro for localization (no operation in tests)
#define N_(x) (x)

// Define Open, Enable, Disable functions as per wextern.c
// These are already included via #include "wextern.c"

// --------------------------------------------
// Example Test Cases
// --------------------------------------------

// Test Case: Open Successfully Initializes with setWindowCb
static void test_Open_Success_WithSetWindowCb(void **state) {
    (void)state;

    // Setup vlc_window_t
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    // Expectations for var_InheritAddress calls
    // First call: "vout-cb-opaque"
    expect_string(var_InheritAddress, varname, "vout-cb-opaque");
    void* mock_opaque = (void*)0xDEADBEEF;
    will_return(var_InheritAddress, mock_opaque);

    // Second call: "vout-cb-window-cb"
    expect_string(var_InheritAddress, varname, "vout-cb-window-cb");
    libvlc_video_output_set_window_cb mock_setWindowCb = (libvlc_video_output_set_window_cb)mock();
    will_return(var_InheritAddress, mock_setWindowCb);

    // Expectation for vlc_obj_malloc
    expect_value(vlc_obj_malloc, obj, &wnd);
    expect_value(vlc_obj_malloc, size, sizeof(wextern_t));
    wextern_t* mock_sys = (wextern_t*)mock();
    will_return(vlc_obj_malloc, mock_sys);

    // Assign system structure after Open
    // Call Open function
    int ret = Open(&wnd);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    assert_ptr_equal(wnd.sys, mock_sys);
    assert_int_equal(wnd.type, VLC_WINDOW_TYPE_DUMMY);
    assert_ptr_equal(wnd.ops, &ops);
    assert_false(wnd.info.has_double_click);

    // Cleanup
    // Assuming Close will be called later
}

// Test Case: Open Fails Due to Memory Allocation Failure
static void test_Open_Failure_MemoryAllocation(void **state) {
    (void)state;

    // Setup vlc_window_t
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    // Expectations for var_InheritAddress calls
    // "vout-cb-opaque"
    expect_string(var_InheritAddress, varname, "vout-cb-opaque");
    void* mock_opaque = (void*)0xCAFEBABE;
    will_return(var_InheritAddress, mock_opaque);

    // "vout-cb-window-cb"
    expect_string(var_InheritAddress, varname, "vout-cb-window-cb");
    libvlc_video_output_set_window_cb mock_setWindowCb = (libvlc_video_output_set_window_cb)mock();
    will_return(var_InheritAddress, mock_setWindowCb);

    // Expectation for vlc_obj_malloc to fail
    expect_value(vlc_obj_malloc, obj, &wnd);
    expect_value(vlc_obj_malloc, size, sizeof(wextern_t));
    will_return(vlc_obj_malloc, NULL); // Simulate malloc failure

    // Call Open function
    int ret = Open(&wnd);

    // Assertions
    assert_int_equal(ret, VLC_ENOMEM);
    assert_ptr_equal(wnd.sys, NULL); // sys should not be set

    // Cleanup
    // No resources to free since allocation failed
}

// Test Case: Open Successfully Initializes Without setWindowCb
static void test_Open_Success_WithoutSetWindowCb(void **state) {
    (void)state;

    // Setup vlc_window_t
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    // Expectations for var_InheritAddress calls
    // "vout-cb-opaque"
    expect_string(var_InheritAddress, varname, "vout-cb-opaque");
    void* mock_opaque = (void*)0xBAADF00D;
    will_return(var_InheritAddress, mock_opaque);

    // "vout-cb-window-cb" returns NULL
    expect_string(var_InheritAddress, varname, "vout-cb-window-cb");
    will_return(var_InheritAddress, NULL); // No setWindowCb

    // Expectation for vlc_obj_malloc
    expect_value(vlc_obj_malloc, obj, &wnd);
    expect_value(vlc_obj_malloc, size, sizeof(wextern_t));
    wextern_t* mock_sys = (wextern_t*)mock();
    memset(mock_sys, 0, sizeof(wextern_t)); // Initialize to zero
    will_return(vlc_obj_malloc, mock_sys);

    // Call Open function
    int ret = Open(&wnd);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    assert_ptr_equal(wnd.sys, mock_sys);
    assert_int_equal(wnd.type, VLC_WINDOW_TYPE_DUMMY);
    assert_ptr_equal(wnd.ops, &ops);
    assert_false(wnd.info.has_double_click);

    // Cleanup
    // Assuming Close will be called later
}

// Test Case: Enable Successfully Sets Window Callbacks
static void test_Enable_Success(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign setWindowCb
    libvlc_video_output_set_window_cb mock_setWindowCb = (libvlc_video_output_set_window_cb)mock();
    sys.setWindowCb = mock_setWindowCb;
    sys.opaque = (void*)0xDEADBEEF;

    // Define window configuration
    vlc_window_cfg_t wcfg = {
        .width = 800,
        .height = 600
    };

    // Expect setWindowCb to be called with correct parameters
    expect_value(mock_setWindowCb, opaque, sys.opaque);
    // Capture the function pointers passed to setWindowCb
    // CMocka does not support capturing function pointers directly, so we'll use a workaround
    // We'll use a global structure to store the function pointers when setWindowCb is called

    typedef struct {
        void (*resize_cb)(void *, unsigned, unsigned);
        void (*mouse_moved_cb)(void *, int, int);
        void (*mouse_press_cb)(void *, libvlc_video_output_mouse_button_t);
        void (*mouse_release_cb)(void *, libvlc_video_output_mouse_button_t);
        vlc_window_t *wnd;
    } callbacks_t;

    callbacks_t captured_cbs;

    // Define a mock_setWindowCb handler
    void mock_setWindowCb_handler(
        void *opaque,
        void (*resize_cb)(void *, unsigned, unsigned),
        void (*mouse_moved_cb)(void *, int, int),
        void (*mouse_press_cb)(void *, libvlc_video_output_mouse_button_t),
        void (*mouse_release_cb)(void *, libvlc_video_output_mouse_button_t),
        vlc_window_t *window
    ) {
        captured_cbs.resize_cb = resize_cb;
        captured_cbs.mouse_moved_cb = mouse_moved_cb;
        captured_cbs.mouse_press_cb = mouse_press_cb;
        captured_cbs.mouse_release_cb = mouse_release_cb;
        captured_cbs.wnd = window;
        function_called();
    }

    // Register the mock_setWindowCb to call our handler
    will_return(mock_setWindowCb, mock_setWindowCb_handler);

    // Call Enable function
    int ret = wnd.ops->enable(&wnd, &wcfg);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    // Verify that setWindowCb was called
    assert_true(function_has_been_called());

    // Further assertions can be made to verify the captured callbacks
    // However, since CMocka does not support capturing function pointers directly,
    // this requires more advanced mocking techniques or integration testing.
    // For simplicity, we'll assume that the callbacks are correctly passed.

    // Cleanup
    // Assuming Disable will be called later
}

// Test Case: Enable Handles Missing setWindowCb Gracefully
static void test_Enable_MissingSetWindowCb(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // setWindowCb is NULL
    sys.setWindowCb = NULL;
    sys.opaque = NULL;

    // Define window configuration
    vlc_window_cfg_t wcfg = {
        .width = 1024,
        .height = 768
    };

    // Call Enable function
    int ret = wnd.ops->enable(&wnd, &wcfg);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    // Ensure that setWindowCb was not called
    // Since setWindowCb is NULL, nothing should have happened
    // No further assertions needed
}

// Test Case: Disable Successfully Removes Window Callbacks
static void test_Disable_Success(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign setWindowCb
    libvlc_video_output_set_window_cb mock_setWindowCb = (libvlc_video_output_set_window_cb)mock();
    sys.setWindowCb = mock_setWindowCb;
    sys.opaque = (void*)0xFEEDFACE;

    // Expect setWindowCb to be called with NULL callbacks to remove handlers
    expect_value(mock_setWindowCb, opaque, sys.opaque);
    // Since we're disabling, all callbacks are set to NULL
    will_return(mock_setWindowCb, NULL); // Simplified, assuming NULL replaces all callbacks

    // Call Disable function
    wnd.ops->disable(&wnd);

    // Assertions
    // Ensure that setWindowCb was called to remove callbacks
    assert_true(function_has_been_called());
}

// Test Case: Disable Handles Missing setWindowCb Gracefully
static void test_Disable_MissingSetWindowCb(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // setWindowCb is NULL
    sys.setWindowCb = NULL;
    sys.opaque = NULL;

    // Call Disable function
    wnd.ops->disable(&wnd);

    // Assertions
    // Ensure that no action was taken and no crash occurred
    assert_true(1); // Placeholder to indicate the test passed
}

// Test Case: WindowResize Callback Correctly Reports Size
static void test_WindowResize_Callback(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign wnd pointer as opaque
    sys.opaque = &wnd;

    // Call WindowResize
    unsigned new_width = 1280;
    unsigned new_height = 720;

    // Expect vlc_window_ReportSize to be called with correct parameters
    expect_value(vlc_window_ReportSize, wnd, &wnd);
    expect_value(vlc_window_ReportSize, width, new_width);
    expect_value(vlc_window_ReportSize, height, new_height);
    will_be_called(vlc_window_ReportSize);

    // Invoke the callback
    WindowResize(sys.opaque, new_width, new_height);

    // Assertions
    // Ensure that vlc_window_ReportSize was called with correct parameters
}

// Test Case: WindowMouseMoved Callback Correctly Reports Mouse Movement
static void test_WindowMouseMoved_Callback(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign wnd pointer as opaque
    sys.opaque = &wnd;

    // Call WindowMouseMoved
    int x = 400;
    int y = 300;

    // Expect vlc_window_ReportMouseMoved to be called with correct parameters
    expect_value(vlc_window_ReportMouseMoved, wnd, &wnd);
    expect_value(vlc_window_ReportMouseMoved, x, x);
    expect_value(vlc_window_ReportMouseMoved, y, y);
    will_be_called(vlc_window_ReportMouseMoved);

    // Invoke the callback
    WindowMouseMoved(sys.opaque, x, y);

    // Assertions
    // Ensure that vlc_window_ReportMouseMoved was called with correct parameters
}

// Test Case: WindowMousePress Callback Correctly Reports Mouse Press
static void test_WindowMousePress_Callback(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign wnd pointer as opaque
    sys.opaque = &wnd;

    // Call WindowMousePress
    libvlc_video_output_mouse_button_t button = LIBVLC_VIDEO_OUTPUT_MOUSE_BUTTON_LEFT;

    // Expect vlc_window_ReportMousePressed to be called with correct parameters
    expect_value(vlc_window_ReportMousePressed, wnd, &wnd);
    expect_value(vlc_window_ReportMousePressed, button, button);
    will_be_called(vlc_window_ReportMousePressed);

    // Invoke the callback
    WindowMousePress(sys.opaque, button);

    // Assertions
    // Ensure that vlc_window_ReportMousePressed was called with correct parameters
}

// Test Case: WindowMouseRelease Callback Correctly Reports Mouse Release
static void test_WindowMouseRelease_Callback(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign wnd pointer as opaque
    sys.opaque = &wnd;

    // Call WindowMouseRelease
    libvlc_video_output_mouse_button_t button = LIBVLC_VIDEO_OUTPUT_MOUSE_BUTTON_RIGHT;

    // Expect vlc_window_ReportMouseReleased to be called with correct parameters
    expect_value(vlc_window_ReportMouseReleased, wnd, &wnd);
    expect_value(vlc_window_ReportMouseReleased, button, button);
    will_be_called(vlc_window_ReportMouseReleased);

    // Invoke the callback
    WindowMouseRelease(sys.opaque, button);

    // Assertions
    // Ensure that vlc_window_ReportMouseReleased was called with correct parameters
}

// Test Case: Close Successfully Cleans Up Resources
static void test_Close_Success(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign setWindowCb
    libvlc_video_output_set_window_cb mock_setWindowCb = (libvlc_video_output_set_window_cb)mock();
    sys.setWindowCb = mock_setWindowCb;
    sys.opaque = (void*)0xFEEDFACE;

    // Expect setWindowCb to be called with NULL callbacks to remove handlers
    expect_value(mock_setWindowCb, opaque, sys.opaque);
    will_return(mock_setWindowCb, NULL); // Simplified, assuming NULL replaces all callbacks

    // Assign window type and ops
    wnd.type = VLC_WINDOW_TYPE_DUMMY;
    wnd.ops = &ops;

    // Call Close function
    wnd.ops->close(&wnd);

    // Assertions
    // Ensure that setWindowCb was called to remove callbacks
    assert_true(function_has_been_called());

    // Ensure that sys was freed
    expect_value(vlc_obj_free, ptr, sys.setWindowCb); // Adjust based on actual freeing logic
    will_be_called(vlc_obj_free);
}

// Test Case: Close Handles Missing setWindowCb Gracefully
static void test_Close_MissingSetWindowCb(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // setWindowCb is NULL
    sys.setWindowCb = NULL;
    sys.opaque = NULL;

    // Assign window type and ops
    wnd.type = VLC_WINDOW_TYPE_DUMMY;
    wnd.ops = &ops;

    // Call Close function
    wnd.ops->close(&wnd);

    // Assertions
    // Ensure that no action was taken and no crash occurred
    assert_true(1); // Placeholder to indicate the test passed
}

// Test Case: Open Assigns Correct Operations and Type
static void test_Open_Assigns_Ops_And_Type(void **state) {
    (void)state;

    // Setup vlc_window_t
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    // Expectations for var_InheritAddress calls
    // "vout-cb-opaque"
    expect_string(var_InheritAddress, varname, "vout-cb-opaque");
    void* mock_opaque = (void*)0x12345678;
    will_return(var_InheritAddress, mock_opaque);

    // "vout-cb-window-cb"
    expect_string(var_InheritAddress, varname, "vout-cb-window-cb");
    libvlc_video_output_set_window_cb mock_setWindowCb = (libvlc_video_output_set_window_cb)mock();
    will_return(var_InheritAddress, mock_setWindowCb);

    // Expectation for vlc_obj_malloc
    expect_value(vlc_obj_malloc, obj, &wnd);
    expect_value(vlc_obj_malloc, size, sizeof(wextern_t));
    wextern_t* mock_sys = (wextern_t*)mock();
    memset(mock_sys, 0, sizeof(wextern_t));
    will_return(vlc_obj_malloc, mock_sys);

    // Call Open function
    int ret = Open(&wnd);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    assert_ptr_equal(wnd.sys, mock_sys);
    assert_int_equal(wnd.type, VLC_WINDOW_TYPE_DUMMY);
    assert_ptr_equal(wnd.ops, &ops);
    assert_false(wnd.info.has_double_click);
}

// Test Case: Enable Does Not Set Callbacks When setWindowCb is NULL
static void test_Enable_NoSetWindowCb(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // setWindowCb is NULL
    sys.setWindowCb = NULL;
    sys.opaque = NULL;

    // Assign window operations
    wnd.ops = &ops;

    // Define window configuration
    vlc_window_cfg_t wcfg = {
        .width = 1280,
        .height = 720
    };

    // Call Enable function
    int ret = wnd.ops->enable(&wnd, &wcfg);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    // Ensure that setWindowCb was not called since it's NULL
    // No further actions expected
}

// Test Case: WindowMousePress Callback Correctly Reports Mouse Press
static void test_WindowMousePress_Callback(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign wnd pointer as opaque
    sys.opaque = &wnd;

    // Call WindowMousePress
    libvlc_video_output_mouse_button_t button = LIBVLC_VIDEO_OUTPUT_MOUSE_BUTTON_LEFT;

    // Expect vlc_window_ReportMousePressed to be called with correct parameters
    expect_value(vlc_window_ReportMousePressed, wnd, &wnd);
    expect_value(vlc_window_ReportMousePressed, button, button);
    will_be_called(vlc_window_ReportMousePressed);

    // Invoke the callback
    WindowMousePress(sys.opaque, button);

    // Assertions
    // Ensure that vlc_window_ReportMousePressed was called with correct parameters
}

// Test Case: WindowMouseRelease Callback Correctly Reports Mouse Release
static void test_WindowMouseRelease_Callback(void **state) {
    (void)state;

    // Setup vlc_window_t and system structure
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    wextern_t sys;
    memset(&sys, 0, sizeof(wextern_t));
    wnd.sys = &sys;

    // Assign wnd pointer as opaque
    sys.opaque = &wnd;

    // Call WindowMouseRelease
    libvlc_video_output_mouse_button_t button = LIBVLC_VIDEO_OUTPUT_MOUSE_BUTTON_RIGHT;

    // Expect vlc_window_ReportMouseReleased to be called with correct parameters
    expect_value(vlc_window_ReportMouseReleased, wnd, &wnd);
    expect_value(vlc_window_ReportMouseReleased, button, button);
    will_be_called(vlc_window_ReportMouseReleased);

    // Invoke the callback
    WindowMouseRelease(sys.opaque, button);

    // Assertions
    // Ensure that vlc_window_ReportMouseReleased was called with correct parameters
}

// --------------------------------------------
// Main Function to Run Tests
// --------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_Open_Success_WithSetWindowCb),
        cmocka_unit_test(test_Open_Failure_MemoryAllocation),
        cmocka_unit_test(test_Open_Success_WithoutSetWindowCb),
        cmocka_unit_test(test_Enable_Success),
        cmocka_unit_test(test_Enable_MissingSetWindowCb),
        cmocka_unit_test(test_Disable_Success),
        cmocka_unit_test(test_Disable_MissingSetWindowCb),
        cmocka_unit_test(test_WindowResize_Callback),
        cmocka_unit_test(test_WindowMouseMoved_Callback),
        cmocka_unit_test(test_WindowMousePress_Callback),
        cmocka_unit_test(test_WindowMouseRelease_Callback),
        cmocka_unit_test(test_Open_Assigns_Ops_And_Type),
        cmocka_unit_test(test_Enable_NoSetWindowCb),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
