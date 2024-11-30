// test_vgl.c

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// Include the vgl.c source here
// For testing purposes, it's recommended to have a header (vgl.h) with necessary declarations.
// However, for this example, we'll assume vgl.c can be included directly.
// Adjust the path as necessary based on your project structure.
#include "vgl.c" // Adjust the path as necessary

// --------------------------------------------
// Mock Definitions for External Dependencies
// --------------------------------------------

// Define minimal structures for testing
typedef struct vlc_object_t vlc_object_t;
typedef struct vlc_gl_cfg vlc_gl_cfg_t;
typedef struct libvlc_video_engine_t libvlc_video_engine_t;
typedef struct libvlc_video_setup_device_cfg_t libvlc_video_setup_device_cfg_t;
typedef struct libvlc_video_setup_device_info_t libvlc_video_setup_device_info_t;
typedef struct libvlc_video_render_cfg_t libvlc_video_render_cfg_t;
typedef struct libvlc_video_output_cfg_t libvlc_video_output_cfg_t;

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

// Mock variable functions
void* var_InheritAddress(vlc_object_t* obj, const char* varname) {
    check_expected(obj);
    check_expected(varname);
    return (void*)mock(); // Return a pointer or NULL
}

// Mock memory allocation functions
void* vlc_obj_calloc(vlc_object_t* obj, size_t nmemb, size_t size) {
    check_expected(obj);
    check_expected(nmemb);
    check_expected(size);
    return (void*)mock(); // Return allocated memory or NULL
}

void vlc_obj_free(void* ptr) {
    check_expected(ptr);
    // No operation needed for the mock
}

// Mock LibVLC callback functions
typedef bool (*libvlc_video_output_cleanup_cb)(void* opaque);
typedef bool (*libvlc_video_output_setup_cb)(void** opaque, libvlc_video_setup_device_cfg_t* setup_cfg, libvlc_video_setup_device_info_t* configured_cfg);
typedef bool (*libvlc_video_update_output_cb)(void* opaque, libvlc_video_render_cfg_t* output_cfg, libvlc_video_output_cfg_t* render_cfg);
typedef void (*libvlc_video_swap_cb)(void* opaque);
typedef bool (*libvlc_video_makeCurrent_cb)(void* opaque, bool make_current);
typedef void* (*libvlc_video_getProcAddress_cb)(void* opaque, const char* name);

// Mock vlc_fourcc_GetCodecFromString
vlc_fourcc_t vlc_fourcc_GetCodecFromString(int codec_type, const char* str) {
    check_expected(codec_type);
    check_expected(str);
    return (vlc_fourcc_t)mock_type(vlc_fourcc_t);
}

// Define VLC macros
#define VLC_SUCCESS 0
#define VLC_ENOMEM -12
#define VLC_EGENERIC -1

#define VLC_UNUSED(x) (void)(x)

#define unlikely(x) __builtin_expect(!!(x), 0)

// Define video_format_t structure
struct video_format {
    int i_chroma;
    int i_width;
    int i_height;
    int orientation;
    // Additional fields as needed
};

// Define vlc_gl_t structure
typedef struct vlc_gl_t {
    const struct vlc_gl_operations* ops;
    struct vout_display_sys_t* sys;
    int api_type; // e.g., VLC_OPENGL, VLC_OPENGL_ES2
    video_orientation_t orientation;
} vlc_gl_t;

// Define video_orientation_t
typedef enum {
    ORIENT_NORMAL = 0,
    ORIENT_ROTATE_90,
    ORIENT_ROTATE_180,
    ORIENT_ROTATE_270,
} video_orientation_t;

// Define libvlc_video_render_cfg_t
struct libvlc_video_render_cfg_t {
    unsigned width;
    unsigned height;
    int bits_per_pixel;
    bool full_range;
    libvlc_video_colorspace_t colorspace;
    libvlc_video_primaries_t primaries;
    libvlc_video_transfer_func_t transfer;
    void* reserved;
};

// Define libvlc_video_output_cfg_t
struct libvlc_video_output_cfg_t {
    int opengl_format;
    bool full_range;
    libvlc_video_colorspace_t colorspace;
    libvlc_video_primaries_t primaries;
    libvlc_video_transfer_func_t transfer;
    libvlc_video_orient_t orientation;
};

// Define libvlc_video_orient_t to match video_orientation_t
typedef enum {
    libvlc_video_orient_top_left = ORIENT_NORMAL,
    libvlc_video_orient_bottom_left = ORIENT_ROTATE_180,
    libvlc_video_orient_top_right = ORIENT_ROTATE_270,
    libvlc_video_orient_bottom_right = ORIENT_ROTATE_90,
} libvlc_video_orient_t;

// Define vout_display_t structure
typedef struct vout_display_t {
    vlc_object_t *obj;
    const struct vlc_display_operations *ops;
    const vout_display_cfg_t *cfg;
    video_format_t *source;
    // Additional fields as needed
} vout_display_t;

// Define vlc_display_operations
typedef struct vlc_display_operations {
    void (*close)(vout_display_t*);
    void (*prepare)(vout_display_t*, picture_t*, const vlc_render_subpicture*, vlc_tick_t);
    void (*display)(vout_display_t*, picture_t*);
    int (*control)(vout_display_t*, int);
} vlc_display_operations_t;

// Define picture_t structure
typedef struct picture {
    uint8_t *p_pixels;
    int i_pitch;
    int i_lines;
    video_format_t format;
    int i_planes;
    struct picture *p_next;
} picture_t;

// Define vlc_tick_t as a 64-bit unsigned integer
typedef uint64_t vlc_tick_t;

// Mock vlc_tick_now() as a mock function
vlc_tick_t vlc_tick_now(void) {
    return (vlc_tick_t)mock_type(vlc_tick_t);
}

// Define vout_display_cfg_t (simplified for testing)
typedef struct vout_display_cfg_t {
    // Add necessary fields as needed
} vout_display_cfg_t;

// --------------------------------------------
// Example Test Cases
// --------------------------------------------

// Test Case: Open Successfully Initializes with All Callbacks
static void test_Open_Success_AllCallbacks(void **state) {
    (void)state;

    // Setup vout_display_t and related structures
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));

    // Mock object and source
    vlc_object_t mock_obj;
    video_format_t source_fmt = { .i_chroma = 0, .i_width = 800, .i_height = 600, .orientation = ORIENT_NORMAL };
    vd.obj = &mock_obj;
    vd.source = &source_fmt;

    // Mock var_InheritAddress to return valid callbacks
    libvlc_video_output_cleanup_cb mock_cleanupCb = (libvlc_video_output_cleanup_cb)mock();
    libvlc_video_output_setup_cb mock_setupCb = (libvlc_video_output_setup_cb)mock();
    libvlc_video_update_output_cb mock_resizeCb = (libvlc_video_update_output_cb)mock();
    libvlc_video_swap_cb mock_swapCb = (libvlc_video_swap_cb)mock();
    libvlc_video_makeCurrent_cb mock_makeCurrentCb = (libvlc_video_makeCurrent_cb)mock();
    libvlc_video_getProcAddress_cb mock_getProcAddressCb = (libvlc_video_getProcAddress_cb)mock();

    // Expect var_InheritAddress calls
    expect_string(var_InheritAddress, varname, "vout-cb-opaque");
    will_return(var_InheritAddress, (void*)0xDEADBEEF); // opaque

    expect_string(var_InheritAddress, varname, "vout-cb-setup");
    will_return(var_InheritAddress, (void*)mock_setupCb);

    expect_string(var_InheritAddress, varname, "vout-cb-cleanup");
    will_return(var_InheritAddress, (void*)mock_cleanupCb);

    expect_string(var_InheritAddress, varname, "vout-cb-update-output");
    will_return(var_InheritAddress, (void*)mock_resizeCb);

    expect_string(var_InheritAddress, varname, "vout-cb-swap");
    will_return(var_InheritAddress, (void*)mock_swapCb);

    expect_string(var_InheritAddress, varname, "vout-cb-make-current");
    will_return(var_InheritAddress, (void*)mock_makeCurrentCb);

    expect_string(var_InheritAddress, varname, "vout-cb-get-proc-address");
    will_return(var_InheritAddress, (void*)mock_getProcAddressCb);

    // Mock vlc_obj_calloc to allocate system structure
    vout_display_sys_t* mock_sys = (vout_display_sys_t*)malloc(sizeof(vout_display_sys_t));
    memset(mock_sys, 0, sizeof(vout_display_sys_t));
    expect_value(vlc_obj_calloc, obj, &mock_obj);
    expect_value(vlc_obj_calloc, nmemb, 1);
    expect_value(vlc_obj_calloc, size, sizeof(vout_display_sys_t));
    will_return(vlc_obj_calloc, mock_sys);

    // Mock setupCb to succeed
    expect_any(mock_setupCb, opaque);
    expect_any(mock_setupCb, setup_cfg);
    expect_any(mock_setupCb, configured_cfg);
    will_return(mock_setupCb, true); // Success

    // Mock resizeCb to succeed
    expect_any(mock_resizeCb, opaque);
    expect_any(mock_resizeCb, output_cfg);
    expect_any(mock_resizeCb, render_cfg);
    will_return(mock_resizeCb, true); // Success

    // Call Open function
    int ret = Open(&vd, 800, 600, NULL); // gl_cfg is unused in Open

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    assert_ptr_equal(vd.ops, &gl_ops); // Ensure operations are set

    // Clean up
    Close(&vd);
    free(mock_sys);
}

// Test Case: Open Fails Due to Missing Callbacks
static void test_Open_Failure_MissingCallbacks(void **state) {
    (void)state;

    // Setup vout_display_t and related structures
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));

    // Mock object and source
    vlc_object_t mock_obj;
    video_format_t source_fmt = { .i_chroma = 0, .i_width = 1024, .i_height = 768, .orientation = ORIENT_NORMAL };
    vd.obj = &mock_obj;
    vd.source = &source_fmt;

    // Mock var_InheritAddress to return NULL for a critical callback (e.g., setupCb)
    expect_string(var_InheritAddress, varname, "vout-cb-opaque");
    will_return(var_InheritAddress, (void*)0xDEADBEEF); // opaque

    expect_string(var_InheritAddress, varname, "vout-cb-setup");
    will_return(var_InheritAddress, NULL); // Missing setupCb

    // Other callbacks can return valid pointers or NULL based on the test
    expect_string(var_InheritAddress, varname, "vout-cb-cleanup");
    will_return(var_InheritAddress, (void*)0xBAADF00D); // Arbitrary non-NULL

    expect_string(var_InheritAddress, varname, "vout-cb-update-output");
    will_return(var_InheritAddress, (void*)0xCAFEBABE); // Arbitrary non-NULL

    expect_string(var_InheritAddress, varname, "vout-cb-swap");
    will_return(var_InheritAddress, (void*)0xFEEDFACE); // Arbitrary non-NULL

    expect_string(var_InheritAddress, varname, "vout-cb-make-current");
    will_return(var_InheritAddress, (void*)0xC0FFEE); // Arbitrary non-NULL

    expect_string(var_InheritAddress, varname, "vout-cb-get-proc-address");
    will_return(var_InheritAddress, (void*)0xBADF00D); // Arbitrary non-NULL

    // Mock vlc_obj_calloc to allocate system structure
    vout_display_sys_t* mock_sys = (vout_display_sys_t*)malloc(sizeof(vout_display_sys_t));
    memset(mock_sys, 0, sizeof(vout_display_sys_t));
    expect_value(vlc_obj_calloc, obj, &mock_obj);
    expect_value(vlc_obj_calloc, nmemb, 1);
    expect_value(vlc_obj_calloc, size, sizeof(vout_display_sys_t));
    will_return(vlc_obj_calloc, mock_sys);

    // Mock setupCb is NULL, so no need to mock its behavior

    // Call Open function
    int ret = Open(&vd, 1024, 768, NULL); // gl_cfg is unused in Open

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
    assert_ptr_equal(vd.ops, NULL); // Operations should not be set due to failure

    // Clean up
    // Since Open failed, Close should still handle cleanup gracefully
    Close(&vd);
    free(mock_sys);
}

// Test Case: MakeCurrent Succeeds
static void test_MakeCurrent_Success(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Mock system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;

    // Mock makeCurrentCb to succeed
    libvlc_video_makeCurrent_cb mock_makeCurrentCb = (libvlc_video_makeCurrent_cb)mock();
    sys.makeCurrentCb = mock_makeCurrentCb;
    sys.opaque = (void*)0xDEADBEEF;

    // Expect makeCurrentCb to be called with make_current = true
    expect_value(mock_makeCurrentCb, opaque, (void*)0xDEADBEEF);
    expect_value(mock_makeCurrentCb, make_current, true);
    will_return(mock_makeCurrentCb, true); // Success

    // Call MakeCurrent
    int ret = MakeCurrent(&gl);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
}

// Test Case: MakeCurrent Fails
static void test_MakeCurrent_Failure(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Mock system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;

    // Mock makeCurrentCb to fail
    libvlc_video_makeCurrent_cb mock_makeCurrentCb = (libvlc_video_makeCurrent_cb)mock();
    sys.makeCurrentCb = mock_makeCurrentCb;
    sys.opaque = (void*)0xDEADBEEF;

    // Expect makeCurrentCb to be called with make_current = true
    expect_value(mock_makeCurrentCb, opaque, (void*)0xDEADBEEF);
    expect_value(mock_makeCurrentCb, make_current, true);
    will_return(mock_makeCurrentCb, false); // Failure

    // Call MakeCurrent
    int ret = MakeCurrent(&gl);

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
}

// Test Case: SwapBuffers Successfully Calls swapCb
static void test_SwapBuffers_Success(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Mock system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;

    // Mock callback functions
    libvlc_video_makeCurrent_cb mock_makeCurrentCb = (libvlc_video_makeCurrent_cb)mock();
    libvlc_video_swap_cb mock_swapCb = (libvlc_video_swap_cb)mock();
    sys.makeCurrentCb = mock_makeCurrentCb;
    sys.swapCb = mock_swapCb;
    sys.opaque = (void*)0xCAFEBABE;

    // Expect MakeCurrent to be called with make_current = true
    expect_value(mock_makeCurrentCb, opaque, (void*)0xCAFEBABE);
    expect_value(mock_makeCurrentCb, make_current, true);
    will_return(mock_makeCurrentCb, true); // Success

    // Expect swapCb to be called
    expect_value(mock_swapCb, opaque, (void*)0xCAFEBABE);
    will_return(mock_swapCb, 0); // No return value

    // Expect MakeCurrent to be called with make_current = false
    expect_value(mock_makeCurrentCb, opaque, (void*)0xCAFEBABE);
    expect_value(mock_makeCurrentCb, make_current, false);
    will_return(mock_makeCurrentCb, true); // Success

    // Call VglSwapBuffers
    VglSwapBuffers(&gl);

    // Assertions
    // Ensure that swapCb was called correctly
}

// Test Case: SwapBuffers Fails During MakeCurrent
static void test_SwapBuffers_Failure_MakeCurrent(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Mock system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;

    // Mock callback functions
    libvlc_video_makeCurrent_cb mock_makeCurrentCb = (libvlc_video_makeCurrent_cb)mock();
    libvlc_video_swap_cb mock_swapCb = (libvlc_video_swap_cb)mock();
    sys.makeCurrentCb = mock_makeCurrentCb;
    sys.swapCb = mock_swapCb;
    sys.opaque = (void*)0xCAFEBABE;

    // Expect MakeCurrent to be called with make_current = true and fail
    expect_value(mock_makeCurrentCb, opaque, (void*)0xCAFEBABE);
    expect_value(mock_makeCurrentCb, make_current, true);
    will_return(mock_makeCurrentCb, false); // Failure

    // Call VglSwapBuffers
    VglSwapBuffers(&gl);

    // Assertions
    // Ensure that swapCb was not called due to MakeCurrent failure
}

// Test Case: Resize Successfully Changes Dimensions
static void test_Resize_Success(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Initial dimensions
    unsigned initial_width = 800;
    unsigned initial_height = 600;

    // Setup system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;
    sys.width = initial_width;
    sys.height = initial_height;

    // Mock callbacks
    libvlc_video_makeCurrent_cb mock_makeCurrentCb = (libvlc_video_makeCurrent_cb)mock();
    libvlc_video_update_output_cb mock_resizeCb = (libvlc_video_update_output_cb)mock();
    sys.makeCurrentCb = mock_makeCurrentCb;
    sys.resizeCb = mock_resizeCb;
    sys.opaque = (void*)0xFEEDFACE;

    // New dimensions
    unsigned new_width = 1024;
    unsigned new_height = 768;

    // Expect MakeCurrent to be called with make_current = true
    expect_value(mock_makeCurrentCb, opaque, (void*)0xFEEDFACE);
    expect_value(mock_makeCurrentCb, make_current, true);
    will_return(mock_makeCurrentCb, true); // Success

    // Expect resizeCb to be called with new dimensions
    libvlc_video_render_cfg_t output_cfg;
    libvlc_video_output_cfg_t render_cfg;
    expect_any(mock_resizeCb, opaque);
    expect_any(mock_resizeCb, output_cfg);
    expect_any(mock_resizeCb, render_cfg);
    // Mock resizeCb to succeed
    will_return(mock_resizeCb, true);

    // Expect MakeCurrent to be called with make_current = false
    expect_value(mock_makeCurrentCb, opaque, (void*)0xFEEDFACE);
    expect_value(mock_makeCurrentCb, make_current, false);
    will_return(mock_makeCurrentCb, true); // Success

    // Call ResizeInternal
    int ret = ResizeInternal(&gl, new_width, new_height);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    assert_int_equal(sys.width, new_width);
    assert_int_equal(sys.height, new_height);
}

// Test Case: Resize Fails Due to resizeCb Failure
static void test_Resize_Failure_ResizeCb(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Initial dimensions
    unsigned initial_width = 800;
    unsigned initial_height = 600;

    // Setup system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;
    sys.width = initial_width;
    sys.height = initial_height;

    // Mock callbacks
    libvlc_video_makeCurrent_cb mock_makeCurrentCb = (libvlc_video_makeCurrent_cb)mock();
    libvlc_video_update_output_cb mock_resizeCb = (libvlc_video_update_output_cb)mock();
    sys.makeCurrentCb = mock_makeCurrentCb;
    sys.resizeCb = mock_resizeCb;
    sys.opaque = (void*)0xFEEDFACE;

    // New dimensions
    unsigned new_width = 1024;
    unsigned new_height = 768;

    // Expect MakeCurrent to be called with make_current = true
    expect_value(mock_makeCurrentCb, opaque, (void*)0xFEEDFACE);
    expect_value(mock_makeCurrentCb, make_current, true);
    will_return(mock_makeCurrentCb, true); // Success

    // Expect resizeCb to be called and fail
    libvlc_video_render_cfg_t output_cfg;
    libvlc_video_output_cfg_t render_cfg;
    expect_any(mock_resizeCb, opaque);
    expect_any(mock_resizeCb, output_cfg);
    expect_any(mock_resizeCb, render_cfg);
    // Mock resizeCb to fail
    will_return(mock_resizeCb, false);

    // Expect MakeCurrent to be called with make_current = false
    expect_value(mock_makeCurrentCb, opaque, (void*)0xFEEDFACE);
    expect_value(mock_makeCurrentCb, make_current, false);
    will_return(mock_makeCurrentCb, true); // Success

    // Call ResizeInternal
    int ret = ResizeInternal(&gl, new_width, new_height);

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
    // Dimensions should remain unchanged
    assert_int_equal(sys.width, initial_width);
    assert_int_equal(sys.height, initial_height);
}

// Test Case: OurGetProcAddress Successfully Retrieves Function Pointer
static void test_OurGetProcAddress_Success(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Mock system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;

    // Mock getProcAddressCb to return a valid address
    libvlc_video_getProcAddress_cb mock_getProcAddressCb = (libvlc_video_getProcAddress_cb)mock();
    sys.getProcAddressCb = mock_getProcAddressCb;
    sys.opaque = (void*)0xBADF00D;

    // Expect getProcAddressCb to be called with a specific name
    const char* func_name = "glGenBuffers";
    expect_value(mock_getProcAddressCb, opaque, (void*)0xBADF00D);
    expect_string(mock_getProcAddressCb, name, func_name);
    void* mock_func_ptr = (void*)0x12345678;
    will_return(mock_getProcAddressCb, mock_func_ptr);

    // Call OurGetProcAddress
    void* retrieved_ptr = OurGetProcAddress(&gl, func_name);

    // Assertions
    assert_ptr_equal(retrieved_ptr, mock_func_ptr);
}

// Test Case: OurGetProcAddress Fails to Retrieve Function Pointer
static void test_OurGetProcAddress_Failure(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Mock system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;

    // Mock getProcAddressCb to return NULL
    libvlc_video_getProcAddress_cb mock_getProcAddressCb = (libvlc_video_getProcAddress_cb)mock();
    sys.getProcAddressCb = mock_getProcAddressCb;
    sys.opaque = (void*)0xBADF00D;

    // Expect getProcAddressCb to be called with a specific name
    const char* func_name = "glNonExistentFunction";
    expect_value(mock_getProcAddressCb, opaque, (void*)0xBADF00D);
    expect_string(mock_getProcAddressCb, name, func_name);
    will_return(mock_getProcAddressCb, NULL); // Function not found

    // Call OurGetProcAddress
    void* retrieved_ptr = OurGetProcAddress(&gl, func_name);

    // Assertions
    assert_ptr_equal(retrieved_ptr, NULL);
}

// Test Case: Close Successfully Calls cleanupCb
static void test_Close_Success(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Mock system structure
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    gl.sys = &sys;

    // Mock cleanupCb
    libvlc_video_output_cleanup_cb mock_cleanupCb = (libvlc_video_output_cleanup_cb)mock();
    sys.cleanupCb = mock_cleanupCb;
    sys.opaque = (void*)0xCAFEBABE;

    // Expect cleanupCb to be called
    expect_value(mock_cleanupCb, opaque, (void*)0xCAFEBABE);
    will_return(mock_cleanupCb, true); // Success

    // Call Close
    Close(&gl);

    // Assertions
    // Ensure that cleanupCb was called
}

// Test Case: Close Without Prior Initialization (Handles Gracefully)
static void test_Close_NoInitialization(void **state) {
    (void)state;

    // Setup vlc_gl_t without initializing sys
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // sys is NULL
    gl.sys = NULL;

    // Call Close
    Close(&gl);

    // Assertions
    // Ensure that Close handles NULL sys gracefully without crashing
}

// Test Case: Resize When Dimensions Do Not Change (No Operation)
static void test_Resize_NoChange(void **state) {
    (void)state;

    // Setup vlc_gl_t and related structures
    vlc_gl_t gl;
    memset(&gl, 0, sizeof(vlc_gl_t));

    // Mock system structure with initial dimensions
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    sys.width = 1280;
    sys.height = 720;
    gl.sys = &sys;

    // Mock callbacks
    libvlc_video_makeCurrent_cb mock_makeCurrentCb = (libvlc_video_makeCurrent_cb)mock();
    libvlc_video_update_output_cb mock_resizeCb = (libvlc_video_update_output_cb)mock();
    sys.makeCurrentCb = mock_makeCurrentCb;
    sys.resizeCb = mock_resizeCb;
    sys.opaque = (void*)0x0BADBEEF;

    // Call ResizeInternal with the same dimensions
    int ret = ResizeInternal(&gl, 1280, 720);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    // Ensure that MakeCurrent and resizeCb were not called
}

// Define mock operation structure (if needed)
static const struct vlc_gl_operations gl_ops = {
    .make_current = MakeCurrent,
    .release_current = ReleaseCurrent,
    .resize = Resize,
    .swap = VglSwapBuffers,
    .get_proc_address = OurGetProcAddress,
    .close = Close,
};

// --------------------------------------------
// Main Function to Run Tests
// --------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_Open_Success_AllCallbacks),
        cmocka_unit_test(test_Open_Failure_MissingCallbacks),
        cmocka_unit_test(test_MakeCurrent_Success),
        cmocka_unit_test(test_MakeCurrent_Failure),
        cmocka_unit_test(test_SwapBuffers_Success),
        cmocka_unit_test(test_SwapBuffers_Failure_MakeCurrent),
        cmocka_unit_test(test_Resize_Success),
        cmocka_unit_test(test_Resize_Failure_ResizeCb),
        cmocka_unit_test(test_OurGetProcAddress_Success),
        cmocka_unit_test(test_OurGetProcAddress_Failure),
        cmocka_unit_test(test_Close_Success),
        cmocka_unit_test(test_Close_NoInitialization),
        cmocka_unit_test(test_Resize_NoChange),
        // Add more tests as needed
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
