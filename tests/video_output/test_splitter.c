// test_splitter.c

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// Include the splitter.c source here
// For testing purposes, it's recommended to have a header (splitter.h) with necessary declarations.
// However, for this example, we'll assume splitter.c can be included directly.
// Adjust the path as necessary based on your project structure.
#include "splitter.c" // Adjust the path as necessary

// --------------------------------------------
// Mock Definitions for External Dependencies
// --------------------------------------------

// Define minimal structures for testing
typedef struct vlc_object_t vlc_object_t;
typedef struct vlc_window_cfg_t vlc_window_cfg_t;
typedef struct vlc_render_subpicture vlc_render_subpicture;
typedef struct vlc_video_context vlc_video_context;
typedef struct vout_display_cfg_t vout_display_cfg_t;
typedef struct video_splitter_output_t video_splitter_output_t;
typedef struct video_splitter_t video_splitter_t;
typedef struct picture picture_t;

// Mock logging functions
void msg_Err(vlc_object_t* obj, const char* fmt, ...) {
    function_called();
}

void msg_Dbg(vlc_object_t* obj, const char* fmt, ...) {
    function_called();
}

void msg_Warn(vlc_object_t* obj, const char* fmt, ...) {
    function_called();
}

void msg_Info(vlc_object_t* obj, const char* fmt, ...) {
    function_called();
}

// Mock variable functions
bool var_InheritBool(vlc_object_t* obj, const char* varname) {
    check_expected(obj);
    check_expected(varname);
    return (bool)mock();
}

char* var_InheritString(vlc_object_t* obj, const char* varname) {
    check_expected(obj);
    check_expected(varname);
    return (char*)mock();
}

// Mock module functions
module_t* module_need(video_splitter_t* splitter, const char* type,
                      const char* name, bool load) {
    check_expected_ptr(splitter);
    check_expected(type);
    check_expected(name);
    check_expected(load);
    return (module_t*)mock();
}

void module_unneed(video_splitter_t* splitter, module_t* module) {
    check_expected_ptr(splitter);
    check_expected_ptr(module);
}

video_splitter_output_t* video_splitter_GetOutputs(video_splitter_t* splitter, int* count) {
    check_expected_ptr(splitter);
    check_expected_ptr(count);
    return (video_splitter_output_t*)mock();
}

// Mock video_splitter functions
int video_splitter_Filter(video_splitter_t* splitter, picture_t** pictures, picture_t* pic) {
    check_expected_ptr(splitter);
    check_expected_ptr(pictures);
    check_expected_ptr(pic);
    return mock_type(int);
}

int video_splitter_Mouse(video_splitter_t* splitter, int part_index,
                         vlc_window_mouse_event_t* event) {
    check_expected_ptr(splitter);
    check_expected(part_index);
    check_expected_ptr(event);
    return mock_type(int);
}

// Mock video_splitter_Create (if applicable)

// Mock window functions
vlc_window_t* vlc_window_New(vlc_object_t* obj, const vlc_window_cfg_t* cfg,
                             const vlc_window_owner_t* owner,
                             const vlc_window_cfg_t* window_cfg) {
    check_expected_ptr(obj);
    check_expected_ptr(cfg);
    check_expected_ptr(owner);
    check_expected_ptr(window_cfg);
    return (vlc_window_t*)mock();
}

int vlc_window_Enable(vlc_window_t* window) {
    check_expected_ptr(window);
    return mock_type(int);
}

void vlc_window_Disable(vlc_window_t* window) {
    check_expected_ptr(window);
    function_called();
}

void vlc_window_Delete(vlc_window_t* window) {
    check_expected_ptr(window);
    function_called();
}

// Mock display functions
vout_display_t* vout_display_New(vlc_object_t* obj, const video_format_t* fmt,
                                 vlc_video_context* ctx, const vout_display_cfg_t* cfg,
                                 const char* module, void* priv) {
    check_expected_ptr(obj);
    check_expected_ptr(fmt);
    check_expected_ptr(ctx);
    check_expected_ptr(cfg);
    check_expected_ptr(module);
    check_expected_ptr(priv);
    return (vout_display_t*)mock();
}

int vout_display_Prepare(vout_display_t* display, picture_t* pic,
                        const vlc_render_subpicture* subpic, vlc_tick_t date) {
    check_expected_ptr(display);
    check_expected_ptr(pic);
    check_expected_ptr(subpic);
    check_expected(date);
    return (int)mock();
}

void vout_display_Display(vout_display_t* display, picture_t* picture) {
    check_expected_ptr(display);
    check_expected_ptr(picture);
}

void vout_display_SetSize(vout_display_t* display, unsigned width, unsigned height) {
    check_expected_ptr(display);
    check_expected(width);
    check_expected(height);
}

void vout_display_Delete(vout_display_t* display) {
    check_expected_ptr(display);
    function_called();
}

// Mock splitter related functions
video_splitter_t* vlc_object_create(vlc_object_t* obj, size_t size) {
    check_expected_ptr(obj);
    check_expected(size);
    return (video_splitter_t*)mock();
}

void vlc_object_delete(video_splitter_t* splitter) {
    check_expected_ptr(splitter);
    function_called();
}

picture_t* picture_Hold(picture_t* pic) {
    check_expected_ptr(pic);
    return pic;
}

void picture_Release(picture_t* pic) {
    check_expected_ptr(pic);
    function_called();
}

int video_splitter_Create(video_splitter_t* splitter, const char* name) {
    check_expected_ptr(splitter);
    check_expected_ptr(name);
    return mock_type(int);
}

int video_splitter_Filter(video_splitter_t* splitter, picture_t** pictures, picture_t* pic) {
    check_expected_ptr(splitter);
    check_expected_ptr(pictures);
    check_expected_ptr(pic);
    return mock_type(int);
}

int video_splitter_Mouse(video_splitter_t* splitter, int part_index,
                         vlc_window_mouse_event_t* event) {
    check_expected_ptr(splitter);
    check_expected(part_index);
    check_expected_ptr(event);
    return mock_type(int);
}

// Mock synchronization primitives
void vlc_sem_init(vlc_sem_t* sem, unsigned int value) {
    check_expected_ptr(sem);
    check_expected(value);
}

void vlc_sem_wait(vlc_sem_t* sem) {
    check_expected_ptr(sem);
    function_called();
}

void vlc_sem_post(vlc_sem_t* sem) {
    check_expected_ptr(sem);
    function_called();
}

void vlc_mutex_init(vlc_mutex_t* mutex) {
    check_expected_ptr(mutex);
}

void vlc_mutex_lock(vlc_mutex_t* mutex) {
    check_expected_ptr(mutex);
    function_called();
}

void vlc_mutex_unlock(vlc_mutex_t* mutex) {
    check_expected_ptr(mutex);
    function_called();
}

void video_splitter_GetDefaultDisplaySize(unsigned* width, unsigned* height,
                                         const video_format_t* source,
                                         const vout_display_cfg_t* cfg) {
    check_expected_ptr(width);
    check_expected_ptr(height);
    check_expected_ptr(source);
    check_expected_ptr(cfg);
    *width = (unsigned)mock_type(unsigned);
    *height = (unsigned)mock_type(unsigned);
}

// Mock video_format functions
void video_format_Copy(video_format_t* dest, const video_format_t* src) {
    check_expected_ptr(dest);
    check_expected_ptr(src);
    memcpy(dest, src, sizeof(video_format_t));
}

void video_format_ApplyRotation(video_format_t* dest, const video_format_t* src) {
    check_expected_ptr(dest);
    check_expected_ptr(src);
    memcpy(dest, src, sizeof(video_format_t));
}

void video_format_Clean(video_format_t* fmt) {
    check_expected_ptr(fmt);
    // No action needed for this mock
}

// Mock vlc_window_Report functions
void vlc_window_ReportMousePressed(vlc_window_t* wnd, unsigned button) {
    function_called();
    check_expected_ptr(wnd);
    check_expected(button);
}

void vlc_window_ReportMouseReleased(vlc_window_t* wnd, unsigned button) {
    function_called();
    check_expected_ptr(wnd);
    check_expected(button);
}

void vlc_window_ReportMouseMoved(vlc_window_t* wnd, int x, int y) {
    function_called();
    check_expected_ptr(wnd);
    check_expected(x);
    check_expected(y);
}

void vlc_window_ReportMouseDoubleClick(vlc_window_t* wnd, unsigned button) {
    function_called();
    check_expected_ptr(wnd);
    check_expected(button);
}

void vlc_window_ReportKeyPress(vlc_window_t* wnd, int key) {
    function_called();
    check_expected_ptr(wnd);
    check_expected(key);
}

void vlc_window_ReportClose(vlc_window_t* wnd) {
    function_called();
    check_expected_ptr(wnd);
}

// Mock vlc_window_SendMouseEvent
void vlc_window_SendMouseEvent(vlc_window_t* wnd, const vlc_window_mouse_event_t* event) {
    function_called();
    check_expected_ptr(wnd);
    check_expected_ptr(event);
}

// Define VLC structures and constants
typedef struct vlc_window_owner_t {
    const struct vlc_window_callbacks* cbs;
    void* sys;
} vlc_window_owner_t;

typedef struct vlc_window_callbacks {
    void (*resized)(vlc_window_t*, unsigned, unsigned, vlc_window_ack_cb, void*);
    void (*closed)(vlc_window_t*);
    void (*mouse_event)(vlc_window_t*, const vlc_window_mouse_event_t*);
    void (*keyboard_event)(vlc_window_t*, unsigned);
} vlc_window_callbacks_t;

typedef void (*vlc_window_ack_cb)(vlc_window_t*, unsigned, unsigned, void*);

typedef struct vlc_vidsplit_part {
    vlc_window_t *window;
    vout_display_t *display;
    vlc_sem_t lock;
    unsigned width;
    unsigned height;
} vlc_vidsplit_part_t;

typedef struct vout_display_sys_t {
    video_splitter_t splitter;
    vlc_mutex_t lock;

    picture_t **pictures;
    struct vlc_vidsplit_part *parts;
} vout_display_sys_t;

// Define video_splitter_t and video_splitter_output_t
struct video_splitter_output_t {
    char* psz_module;
    video_format_t fmt;
};

struct video_splitter_t {
    video_splitter_output_t* p_output;
    int i_output;
    video_format_t fmt;
    module_t* p_module;
};

// Define vout_display_t
typedef struct vout_display_t {
    vout_display_sys_t *sys;
    const struct vlc_display_operations *ops;
    const vout_display_cfg_t *cfg;
    // Additional fields as needed
} vout_display_t;

// Define vlc_display_operations
typedef struct vlc_display_operations {
    void (*close)(vout_display_t*);
    void (*prepare)(vout_display_t*, picture_t*, const vlc_render_subpicture*, vlc_tick_t);
    void (*display)(vout_display_t*, picture_t*);
    int (*control)(vout_display_t*, int);
} vlc_display_operations_t;

// Define control queries
#define VOUT_DISPLAY_CHANGE_DISPLAY_SIZE 1
#define VOUT_DISPLAY_CHANGE_SOURCE_ASPECT 2
#define VOUT_DISPLAY_CHANGE_SOURCE_CROP 3
#define VOUT_DISPLAY_CHANGE_SOURCE_PLACE 4

// Define picture_t structure
struct picture {
    uint8_t *p_pixels;
    int i_pitch;
    int i_lines;
    video_format_t format;
    int i_planes;
    struct picture *p_next;
} __attribute__((aligned(8)));

// Define module_t as opaque
typedef struct module_t module_t;

// Define VLC macros
#define VLC_ENOMEM -12
#define VLC_EGENERIC -1
#define VLC_SUCCESS 0

#define VLC_UNUSED(x) (void)(x)

#define unlikely(x) __builtin_expect(!!(x), 0)

// Define test-specific structures
typedef struct vlc_window_t {
    vlc_window_owner_t owner;
    // Additional fields as needed
} vlc_window_t;

// --------------------------------------------
// Example Test Cases
// --------------------------------------------

// Test Case: Open Successfully Initializes Splitter and Displays Outputs
static void test_splitter_Open_Success(void **state) {
    (void)state;

    // Setup vout_display_t and related structures
    vout_display_t vd;
    vout_display_sys_t *sys;
    memset(&vd, 0, sizeof(vout_display_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_display_sys_t));
    void* mock_sys = malloc(sizeof(vout_display_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_display_sys_t));

    vd.sys = sys = mock_sys;
    vd.ops = NULL; // Will be set by Open
    vd.cfg = NULL; // Adjust if needed

    // Mock var_InheritString for "video-splitter" to return "default_splitter"
    expect_any(var_InheritString, obj);
    expect_string(var_InheritString, varname, "video-splitter");
    will_return(var_InheritString, strdup("default_splitter"));

    // Mock module_need to return a valid module
    module_t mock_module;
    expect_any(module_need, splitter);
    expect_string(module_need, type, "video splitter");
    expect_string(module_need, name, "default_splitter");
    expect_value(module_need, load, true);
    will_return(module_need, &mock_module);

    // Mock vlc_object_create to return the splitter
    expect_any(vlc_object_create, obj);
    expect_value(vlc_object_create, size, sizeof(video_splitter_t));
    will_return(vlc_object_create, &sys->splitter);

    // Mock module_unneed to be called during Close
    expect_ptr(module_unneed, splitter, &sys->splitter);
    expect_ptr(module_unneed, module, &mock_module);

    // Mock video_splitter_Create to succeed
    expect_any(video_splitter_Create, splitter);
    expect_string(video_splitter_Create, name, "default_splitter");
    will_return(video_splitter_Create, 0); // Success

    // Mock video_splitter_Filter to succeed
    expect_any(video_splitter_Filter, splitter);
    expect_any(video_splitter_Filter, pictures);
    expect_any(video_splitter_Filter, pic);
    will_return(video_splitter_Filter, 0); // Success

    // Mock video_splitter_Mouse to succeed
    expect_any(video_splitter_Mouse, splitter);
    expect_any(video_splitter_Mouse, part_index);
    expect_any(video_splitter_Mouse, event);
    will_return(video_splitter_Mouse, 0); // Success

    // Mock video_splitter_GetDefaultDisplaySize
    expect_any(video_splitter_GetDefaultDisplaySize, width);
    expect_any(video_splitter_GetDefaultDisplaySize, height);
    expect_any(video_splitter_GetDefaultDisplaySize, source);
    expect_any(video_splitter_GetDefaultDisplaySize, cfg);
    will_return(video_splitter_GetDefaultDisplaySize, 1920); // width
    will_return(video_splitter_GetDefaultDisplaySize, 1080); // height

    // Mock video_splitter_p_output
    sys->splitter.i_output = 2;
    video_splitter_output_t outputs[2] = {
        { "splitter_module_1", { .i_width = 1920, .i_height = 1080, .orientation = 0 } },
        { "splitter_module_2", { .i_width = 1280, .i_height = 720, .orientation = 0 } }
    };
    sys->splitter.p_output = outputs;

    // Mock vlc_object_create for splitter
    expect_any(vlc_object_create, obj);
    expect_value(vlc_object_create, size, sizeof(video_splitter_t));
    will_return(vlc_object_create, &sys->splitter);

    // Mock video_splitter_CreateWindow for each part
    for (int i = 0; i < sys->splitter.i_output; i++) {
        vlc_window_t* mock_window = (vlc_window_t*)mock();
        expect_any(video_splitter_CreateWindow, obj);
        expect_any(video_splitter_CreateWindow, vdcfg);
        expect_any(video_splitter_CreateWindow, source);
        expect_any(video_splitter_CreateWindow, sys->parts);
        will_return(video_splitter_CreateWindow, mock_window);

        // Mock vlc_window_Enable to succeed
        expect_any(vlc_window_Enable, window);
        will_return(vlc_window_Enable, 0); // Success

        // Mock vout_display_New to return a valid display
        vout_display_t* mock_display = (vout_display_t*)mock();
        expect_any(vout_display_New, obj);
        expect_any(vout_display_New, fmt);
        expect_any(vout_display_New, ctx);
        expect_any(vout_display_New, cfg);
        expect_any(vout_display_New, module);
        expect_any(vout_display_New, priv);
        will_return(vout_display_New, mock_display);
    }

    // Mock vlc_vidsplit_Open to set ops
    // Since Open is defined within splitter.c and is being tested, ensure ops are set

    // Call Open function
    video_format_t fmt;
    memset(&fmt, 0, sizeof(video_format_t));
    int ret = vlc_vidsplit_Open(&vd, &fmt, NULL);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    assert_non_null(vd.sys);
    assert_non_null(vd.ops);
    // Additional assertions can be added to verify splitter state

    // Cleanup
    vlc_vidsplit_Close(&vd);
    free(var_InheritString(NULL, "video-splitter")); // Free duplicated string
}

// Test Case: Open Fails Due to Module Load Failure
static void test_splitter_Open_Failure_ModuleLoad(void **state) {
    (void)state;

    // Setup vout_display_t and related structures
    vout_display_t vd;
    vout_display_sys_t *sys;
    memset(&vd, 0, sizeof(vout_display_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_display_sys_t));
    void* mock_sys = malloc(sizeof(vout_display_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_display_sys_t));

    vd.sys = sys = mock_sys;
    vd.ops = NULL; // Will be set by Open
    vd.cfg = NULL; // Adjust if needed

    // Mock var_InheritString for "video-splitter" to return "invalid_splitter"
    expect_any(var_InheritString, obj);
    expect_string(var_InheritString, varname, "video-splitter");
    will_return(var_InheritString, strdup("invalid_splitter"));

    // Mock module_need to fail (return NULL)
    expect_any(module_need, splitter);
    expect_string(module_need, type, "video splitter");
    expect_string(module_need, name, "invalid_splitter");
    expect_value(module_need, load, true);
    will_return(module_need, NULL);

    // Call Open function
    video_format_t fmt;
    memset(&fmt, 0, sizeof(video_format_t));
    int ret = vlc_vidsplit_Open(&vd, &fmt, NULL);

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
    assert_non_null(vd.sys);
    // Ensure that sys->splitter.p_module is NULL

    // Cleanup
    free(var_InheritString(NULL, "video-splitter")); // Free duplicated string
}

// Test Case: Close Without Prior Open (Should Handle Gracefully)
static void test_splitter_Close_NoPriorOpen(void **state) {
    (void)state;

    // Setup vout_display_t without initializing sys
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));

    vd.ops = &ops; // Assuming ops is defined in splitter.c
    vd.cfg = NULL; // Adjust if needed

    // Call Close function
    vlc_vidsplit_Close(&vd);

    // Assertions
    // Ensure that Close handles NULL sys gracefully
    // No assertions needed, just ensure no crash
}

// Test Case: Display Function Successfully Displays Pictures
static void test_splitter_Display_Success(void **state) {
    (void)state;

    // Setup vout_display_t and related structures
    vout_display_t vd;
    vout_display_sys_t *sys;
    memset(&vd, 0, sizeof(vout_display_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_display_sys_t));
    void* mock_sys = malloc(sizeof(vout_display_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_display_sys_t));

    vd.sys = sys = mock_sys;
    vd.ops = &ops;
    vd.cfg = NULL; // Adjust if needed

    // Initialize splitter with 2 outputs
    sys->splitter.i_output = 2;
    sys->pictures = vlc_obj_malloc(NULL, 2 * sizeof(picture_t*));
    sys->parts = vlc_obj_malloc(NULL, 2 * sizeof(vlc_vidsplit_part_t));

    // Initialize parts
    for (int i = 0; i < 2; i++) {
        vlc_sem_init(&sys->parts[i].lock, 1);
        sys->parts[i].display = (vout_display_t*)mock(); // Mock display
        sys->parts[i].window = (vlc_window_t*)mock(); // Mock window
        sys->parts[i].width = 1920;
        sys->parts[i].height = 1080;
    }

    // Mock video_splitter_Filter to succeed
    expect_any(video_splitter_Filter, splitter);
    expect_any(video_splitter_Filter, pictures);
    expect_any(video_splitter_Filter, pic);
    will_return(video_splitter_Filter, 0); // Success

    // Mock picture_Hold
    expect_any(picture_Hold, pic);
    will_return(picture_Hold, (picture_t*)mock());

    // Call Prepare function
    picture_t pic;
    memset(&pic, 0, sizeof(picture_t));
    vlc_vidsplit_Prepare(&vd, &pic, NULL, 0);

    // Mock vout_display_Prepare for each display
    for (int i = 0; i < 2; i++) {
        vout_display_t* mock_display = sys->parts[i].display;
        expect_any(vout_display_Prepare, display);
        expect_any(vout_display_Prepare, pic);
        expect_any(vout_display_Prepare, subpic);
        expect_any(vout_display_Prepare, date);
        will_return(vout_display_Prepare, (int)mock());
    }

    // Mock vout_display_Display for each display
    for (int i = 0; i < 2; i++) {
        vout_display_t* mock_display = sys->parts[i].display;
        picture_t* mock_picture = sys->pictures[i];
        expect_any(vout_display_Display, display);
        expect_any(vout_display_Display, picture);
        will_return(vout_display_Display, 0); // Success

        // Mock picture_Release
        expect_any(picture_Release, pic);
        function_called();
    }

    // Call Display function
    vlc_vidsplit_Display(&vd, &pic);

    // Assertions
    // Ensure that displays were called correctly

    // Cleanup
    vlc_vidsplit_Close(&vd);
    free(sys->pictures);
    free(sys->parts);
}

// Test Case: Control Operation with Unsupported Query
static void test_splitter_Control_UnsupportedQuery(void **state) {
    (void)state;

    // Setup vout_display_t and related structures
    vout_display_t vd;
    vout_display_sys_t *sys;
    memset(&vd, 0, sizeof(vout_display_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_display_sys_t));
    void* mock_sys = malloc(sizeof(vout_display_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_display_sys_t));

    vd.sys = sys = mock_sys;
    vd.ops = &ops;
    vd.cfg = NULL; // Adjust if needed

    // Define unsupported query
    int query = 9999;

    // Expect msg_Err to be called for unsupported query
    expect_any(msg_Err, obj);
    expect_any(msg_Err, fmt);

    // Call Control function with unsupported query
    int ret = vd.ops->control(&vd, query);

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
}

// Test Case: Control Operation with Supported Query (CHANGE_DISPLAY_SIZE)
static void test_splitter_Control_Supported_DisplaySize(void **state) {
    (void)state;

    // Setup vout_display_t and related structures
    vout_display_t vd;
    vout_display_sys_t *sys;
    memset(&vd, 0, sizeof(vout_display_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_display_sys_t));
    void* mock_sys = malloc(sizeof(vout_display_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_display_sys_t));

    vd.sys = sys = mock_sys;
    vd.ops = &ops;
    vd.cfg = NULL; // Adjust if needed

    // Initialize splitter with 2 outputs
    sys->splitter.i_output = 2;
    sys->pictures = vlc_obj_malloc(NULL, 2 * sizeof(picture_t*));
    sys->parts = vlc_obj_malloc(NULL, 2 * sizeof(vlc_vidsplit_part_t));

    // Initialize parts
    for (int i = 0; i < 2; i++) {
        vlc_sem_init(&sys->parts[i].lock, 1);
        sys->parts[i].display = (vout_display_t*)mock(); // Mock display
        sys->parts[i].window = (vlc_window_t*)mock(); // Mock window
        sys->parts[i].width = 1920;
        sys->parts[i].height = 1080;
    }

    // Define new display size
    unsigned new_width = 1280;
    unsigned new_height = 720;

    // Expect vout_display_SetSize to be called for each display
    for (int i = 0; i < 2; i++) {
        vout_display_t* mock_display = sys->parts[i].display;
        expect_any(vout_display_SetSize, display);
        expect_value(vout_display_SetSize, width, new_width);
        expect_value(vout_display_SetSize, height, new_height);
    }

    // Call Control function with VOUT_DISPLAY_CHANGE_DISPLAY_SIZE
    int ret = vd.ops->control(&vd, VOUT_DISPLAY_CHANGE_DISPLAY_SIZE);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
}

// Test Case: Display Function Handles NULL Pictures Gracefully
static void test_splitter_Display_NullPictures(void **state) {
    (void)state;

    // Setup vout_display_t and related structures
    vout_display_t vd;
    vout_display_sys_t *sys;
    memset(&vd, 0, sizeof(vout_display_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_display_sys_t));
    void* mock_sys = malloc(sizeof(vout_display_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_display_sys_t));

    vd.sys = sys = mock_sys;
    vd.ops = &ops;
    vd.cfg = NULL; // Adjust if needed

    // Initialize splitter with 2 outputs
    sys->splitter.i_output = 2;
    sys->pictures = vlc_obj_malloc(NULL, 2 * sizeof(picture_t*));
    sys->parts = vlc_obj_malloc(NULL, 2 * sizeof(vlc_vidsplit_part_t));

    // Initialize parts with NULL displays
    for (int i = 0; i < 2; i++) {
        vlc_sem_init(&sys->parts[i].lock, 1);
        sys->parts[i].display = NULL; // No display
        sys->parts[i].window = (vlc_window_t*)mock(); // Mock window
        sys->parts[i].width = 1920;
        sys->parts[i].height = 1080;
    }

    // Mock video_splitter_Filter to succeed
    expect_any(video_splitter_Filter, splitter);
    expect_any(video_splitter_Filter, pictures);
    expect_any(video_splitter_Filter, pic);
    will_return(video_splitter_Filter, 0); // Success

    // Mock picture_Hold
    expect_any(picture_Hold, pic);
    will_return(picture_Hold, (picture_t*)mock());

    // Call Prepare function with a dummy picture
    picture_t pic;
    memset(&pic, 0, sizeof(picture_t));
    vlc_vidsplit_Prepare(&vd, &pic, NULL, 0);

    // Since displays are NULL, vout_display_Prepare should not be called

    // Mock vout_display_Display should not be called
    // Therefore, no expectations set for vout_display_Display

    // Call Display function
    vlc_vidsplit_Display(&vd, &pic);

    // Assertions
    // Ensure that displays were not called and function handled NULL gracefully

    // Cleanup
    vlc_vidsplit_Close(&vd);
    free(sys->pictures);
    free(sys->parts);
}

// Define vlc_display_operations
static const struct vlc_display_operations ops = {
    .close = vlc_vidsplit_Close,
    .prepare = vlc_vidsplit_Prepare,
    .display = vlc_vidsplit_Display,
    .control = vlc_vidsplit_Control,
};

// --------------------------------------------
// Main Function to Run Tests
// --------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_splitter_Open_Success),
        cmocka_unit_test(test_splitter_Open_Failure_ModuleLoad),
        cmocka_unit_test(test_splitter_Close_NoPriorOpen),
        cmocka_unit_test(test_splitter_Display_Success),
        cmocka_unit_test(test_splitter_Control_UnsupportedQuery),
        cmocka_unit_test(test_splitter_Control_Supported_DisplaySize),
        cmocka_unit_test(test_splitter_Display_NullPictures),
        // Add more tests as needed
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
