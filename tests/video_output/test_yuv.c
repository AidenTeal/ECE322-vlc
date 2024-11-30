// test_yuv.c

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// Include the yuv.c source here
// For testing purposes, it's recommended to have a header (yuv.h) with necessary declarations.
// However, for this example, we'll assume yuv.c can be included directly.
// Adjust the path as necessary based on your project structure.
#include "yuv.c" // Adjust the path as necessary

// --------------------------------------------
// Mock Definitions for External Dependencies
// --------------------------------------------

// Define minimal structures for testing
typedef struct vlc_object_t vlc_object_t;
typedef struct vlc_window_cfg_t vlc_window_cfg_t;
typedef struct vlc_vout_display_t vout_display_t;
typedef struct video_format_t video_format_t;

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
// Not used directly in yuv.c, but kept for completeness
void vlc_window_ReportSize(vout_display_t* wnd, unsigned width, unsigned height) {
    check_expected_ptr(wnd);
    check_expected(width);
    check_expected(height);
    function_called();
}

void vlc_window_ReportMouseMoved(vout_display_t* wnd, int x, int y) {
    check_expected_ptr(wnd);
    check_expected(x);
    check_expected(y);
    function_called();
}

void vlc_window_ReportMousePressed(vout_display_t* wnd, unsigned button) {
    check_expected_ptr(wnd);
    check_expected(button);
    function_called();
}

void vlc_window_ReportMouseReleased(vout_display_t* wnd, unsigned button) {
    check_expected_ptr(wnd);
    check_expected(button);
    function_called();
}

void vlc_window_ReportMouseDoubleClick(vout_display_t* wnd, unsigned button) {
    check_expected_ptr(wnd);
    check_expected(button);
    function_called();
}

void vlc_window_ReportKeyPress(vout_display_t* wnd, int key) {
    check_expected_ptr(wnd);
    check_expected(key);
    function_called();
}

void vlc_window_ReportClose(vout_display_t* wnd) {
    check_expected_ptr(wnd);
    function_called();
}

// Mock variable functions
void* var_InheritAddress(vlc_object_t* obj, const char* varname) {
    check_expected_ptr(obj);
    check_expected(varname);
    return (void*)mock(); // Return a pointer or NULL
}

bool var_InheritBool(vout_display_t* vd, const char* varname) {
    check_expected_ptr(vd);
    check_expected(varname);
    return (bool)mock_type(int); // Return a boolean
}

char* var_InheritString(vout_display_t* vd, const char* varname) {
    check_expected_ptr(vd);
    check_expected(varname);
    return (char*)mock(); // Return a string or NULL
}

// Mock memory allocation functions
void* vlc_fopen(const char* filename, const char* mode) {
    check_expected_ptr(filename);
    check_expected_ptr(mode);
    return (void*)mock(); // Return a FILE pointer or NULL
}

void* malloc_wrapper(size_t size) {
    check_expected(size);
    return (void*)mock(); // Return allocated memory or NULL
}

void free_wrapper(void* ptr) {
    check_expected(ptr);
    // No operation needed for the mock
}

// Mock VLC fourcc functions
vlc_fourcc_t vlc_fourcc_GetCodecFromString(int category, const char* name) {
    check_expected(category);
    check_expected_ptr(name);
    return (vlc_fourcc_t)mock_type(vlc_fourcc_t); // Return a fourcc code or 0
}

// Define VLC macros
#define VLC_SUCCESS 0
#define VLC_ENOMEM -12
#define VLC_EGENERIC -1

#define VLC_UNUSED(x) (void)(x)

// Define custom types
typedef unsigned int vlc_fourcc_t;

// Define video_format structure (from VLC)
struct video_format_t {
    int i_chroma;
    int i_visible_width;
    int i_visible_height;
    int i_frame_rate;
    int i_frame_rate_base;
    int i_sar_num;
    int i_sar_den;
    // Additional fields as needed
};

// Define vout_display_sys_t structure (from yuv.c)
typedef struct vout_display_sys_t {
    FILE *f;
    bool  is_first;
    bool  is_yuv4mpeg2;
} vout_display_sys_t;

// Define window structure
struct vlc_vout_display_t {
    const struct vlc_display_operations* ops;
    void* sys;
    video_format_t fmt;
    // Additional fields as needed
};

// Define function prototypes (from yuv.c)
static int Open(vout_display_t *vd, video_format_t *fmtp, vlc_video_context *context);
static void Close(vout_display_t *vd);
static void Display(vout_display_t *vd, picture_t *picture);
static int Control(vout_display_t *vd, int query);

// Define picture structure (from VLC)
typedef struct picture_t {
    int i_planes;
    struct plane {
        uint8_t *p_pixels;
        int i_visible_pitch;
        int i_pitch;
        int i_visible_lines;
    } p[3]; // Assuming up to 3 planes (e.g., Y, U, V)
    bool b_progressive;
    bool b_top_field_first;
} picture_t;

// Define vlc_display_operations structure (from yuv.c)
struct vlc_display_operations {
    void (*close)(vout_display_t*);
    void (*display)(vout_display_t*, picture_t*);
    int  (*control)(vout_display_t*, int);
};

// Define YUV file prefix
#define CFG_PREFIX "yuv-"

// --------------------------------------------
// Example Test Cases
// --------------------------------------------

// Helper function to simulate VLC's var_InheritBool and var_InheritString
static void setup_var_inherit(bool yuv4mpeg2, const char* file, const char* chroma) {
    // Set up var_InheritBool for "yuv-yuv4mpeg2"
    will_return(var_InheritBool, yuv4mpeg2);
    
    // Set up var_InheritString for "yuv-file" and "yuv-chroma"
    will_return(var_InheritString, (char*)file);
    will_return(var_InheritString, (char*)chroma);
}

// Test Case: Open Successfully Initializes with YUV4MPEG2 Header
static void test_Open_Success_WithYUV4MPEG2(void **state) {
    (void)state;
    
    // Setup vout_display_t
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    // Expectations for var_InheritBool and var_InheritString
    expect_any(var_InheritBool, vd);
    expect_string(var_InheritBool, varname, "yuv-yuv4mpeg2");
    will_return(var_InheritBool, 1); // Enable YUV4MPEG2
    
    expect_any(var_InheritString, vd);
    expect_string(var_InheritString, varname, "yuv-file");
    will_return(var_InheritString, "output.yuv"); // Valid file
    
    expect_any(var_InheritString, vd);
    expect_string(var_InheritString, varname, "yuv-chroma");
    will_return(var_InheritString, "I420"); // Supported chroma
    
    // Mock vlc_fourcc_GetCodecFromString to return VLC_CODEC_I420
    expect_value(vlc_fourcc_GetCodecFromString, category, VIDEO_ES);
    expect_string(vlc_fourcc_GetCodecFromString, name, "I420");
    will_return(vlc_fourcc_GetCodecFromString, VLC_CODEC_I420);
    
    // Mock vlc_fopen to return a valid FILE pointer
    expect_string(vlc_fopen, filename, "output.yuv");
    expect_string(vlc_fopen, mode, "wb");
    FILE mock_file;
    will_return(vlc_fopen, &mock_file);
    
    // Call Open function
    video_format_t fmt;
    int ret = Open(&vd, &fmt, NULL);
    
    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    vout_display_sys_t *sys = (vout_display_sys_t*)vd.sys;
    assert_non_null(sys);
    assert_true(sys->is_yuv4mpeg2);
    assert_false(sys->is_first);
    assert_ptr_equal(sys->f, &mock_file);
    
    // Further assertions can check the initialized video format
    assert_int_equal(fmt.i_chroma, VLC_CODEC_I420);
    assert_int_equal(fmt.i_visible_width, 0); // Default values
    assert_int_equal(fmt.i_visible_height, 0);
}

// Test Case: Open Fails Due to Memory Allocation Failure
static void test_Open_Failure_MemoryAllocation(void **state) {
    (void)state;
    
    // Setup vout_display_t
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    // Expectations for var_InheritBool and var_InheritString
    expect_any(var_InheritBool, vd);
    expect_string(var_InheritBool, varname, "yuv-yuv4mpeg2");
    will_return(var_InheritBool, 0); // Disable YUV4MPEG2
    
    expect_any(var_InheritString, vd);
    expect_string(var_InheritString, varname, "yuv-file");
    will_return(var_InheritString, "output.yuv"); // Valid file
    
    expect_any(var_InheritString, vd);
    expect_string(var_InheritString, varname, "yuv-chroma");
    will_return(var_InheritString, "I420"); // Supported chroma
    
    // Mock vlc_fourcc_GetCodecFromString to return VLC_CODEC_I420
    expect_value(vlc_fourcc_GetCodecFromString, category, VIDEO_ES);
    expect_string(vlc_fourcc_GetCodecFromString, name, "I420");
    will_return(vlc_fourcc_GetCodecFromString, VLC_CODEC_I420);
    
    // Mock malloc to fail
    expect_value(malloc_wrapper, size, sizeof(vout_display_sys_t));
    will_return(malloc_wrapper, NULL); // Simulate malloc failure
    
    // Call Open function
    video_format_t fmt;
    int ret = Open(&vd, &fmt, NULL);
    
    // Assertions
    assert_int_equal(ret, VLC_ENOMEM);
    assert_null(vd.sys);
}

// Test Case: Open Fails Due to File Opening Failure
static void test_Open_Failure_FileOpening(void **state) {
    (void)state;
    
    // Setup vout_display_t
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    // Expectations for var_InheritBool and var_InheritString
    expect_any(var_InheritBool, vd);
    expect_string(var_InheritBool, varname, "yuv-yuv4mpeg2");
    will_return(var_InheritBool, 1); // Enable YUV4MPEG2
    
    expect_any(var_InheritString, vd);
    expect_string(var_InheritString, varname, "yuv-file");
    will_return(var_InheritString, "invalid/output.yuv"); // Invalid file
    
    expect_any(var_InheritString, vd);
    expect_string(var_InheritString, varname, "yuv-chroma");
    will_return(var_InheritString, "I420"); // Supported chroma
    
    // Mock vlc_fourcc_GetCodecFromString to return VLC_CODEC_I420
    expect_value(vlc_fourcc_GetCodecFromString, category, VIDEO_ES);
    expect_string(vlc_fourcc_GetCodecFromString, name, "I420");
    will_return(vlc_fourcc_GetCodecFromString, VLC_CODEC_I420);
    
    // Mock vlc_fopen to fail
    expect_string(vlc_fopen, filename, "invalid/output.yuv");
    expect_string(vlc_fopen, mode, "wb");
    will_return(vlc_fopen, NULL); // Simulate fopen failure
    
    // Call Open function
    video_format_t fmt;
    int ret = Open(&vd, &fmt, NULL);
    
    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
    assert_null(vd.sys);
}

// Test Case: Open Fails with Unsupported Chroma Format in YUV4MPEG2 Mode
static void test_Open_Failure_UnsupportedChroma_YUV4MPEG2(void **state) {
    (void)state;
    
    // Setup vout_display_t
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    // Expectations for var_InheritBool and var_InheritString
    expect_any(var_InheritBool, vd);
    expect_string(var_InheritBool, varname, "yuv-yuv4mpeg2");
    will_return(var_InheritBool, 1); // Enable YUV4MPEG2
    
    expect_any(var_InheritString, vd);
    expect_string(var_InheritString, varname, "yuv-file");
    will_return(var_InheritString, "output.yuv"); // Valid file
    
    expect_any(var_InheritString, vd);
    expect_string(var_InheritString, varname, "yuv-chroma");
    will_return(var_InheritString, "RGB"); // Unsupported chroma
    
    // Mock vlc_fourcc_GetCodecFromString to return VLC_CODEC_RGB24
    expect_value(vlc_fourcc_GetCodecFromString, category, VIDEO_ES);
    expect_string(vlc_fourcc_GetCodecFromString, name, "RGB");
    will_return(vlc_fourcc_GetCodecFromString, VLC_CODEC_RGB24);
    
    // Mock vlc_fopen to return a valid FILE pointer
    expect_string(vlc_fopen, filename, "output.yuv");
    expect_string(vlc_fopen, mode, "wb");
    FILE mock_file;
    will_return(vlc_fopen, &mock_file);
    
    // Call Open function
    video_format_t fmt;
    int ret = Open(&vd, &fmt, NULL);
    
    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
    assert_null(vd.sys);
}

// Test Case: Display Writes YUV4MPEG2 Header on First Frame
static void test_Display_FirstFrame_WithYUV4MPEG2(void **state) {
    (void)state;
    
    // Setup vout_display_t and system structure
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    sys.is_yuv4mpeg2 = true;
    sys.is_first = false;
    FILE mock_file;
    sys.f = &mock_file;
    vd.sys = &sys;
    
    // Assign window operations
    vd.ops = &ops;
    
    // Define video format
    vd.fmtp.i_chroma = VLC_CODEC_I420;
    vd.fmtp.i_visible_width = 640;
    vd.fmtp.i_visible_height = 480;
    vd.fmtp.i_frame_rate = 25;
    vd.fmtp.i_frame_rate_base = 1;
    vd.fmtp.i_sar_num = 1;
    vd.fmtp.i_sar_den = 1;
    
    // Mock var_InheritBool and var_InheritString if needed
    // Not necessary here
    
    // Mock fwrite for header and frame data
    expect_any(fwrite, ptr);
    expect_any(fwrite, size);
    expect_any(fwrite, nmemb);
    expect_any(fwrite, stream);
    will_return(fwrite, 0); // Simulate successful fwrite
    
    // Mock fflush to return 0 (success)
    expect_any(fflush, stream);
    will_return(fflush, 0);
    
    // Create a mock picture
    picture_t picture;
    memset(&picture, 0, sizeof(picture_t));
    picture.b_progressive = true;
    picture.b_top_field_first = true;
    picture.i_planes = 3;
    
    // Initialize plane data
    for(int i = 0; i < picture.i_planes; i++) {
        picture.p[i].p_pixels = malloc(100); // Dummy data
        picture.p[i].i_visible_pitch = 100;
        picture.p[i].i_pitch = 100;
        picture.p[i].i_visible_lines = 1;
    }
    
    // Call Display function
    Display(&vd, &picture);
    
    // Assertions
    assert_true(sys.is_first);
    
    // Cleanup
    for(int i = 0; i < picture.i_planes; i++) {
        free(picture.p[i].p_pixels);
    }
}

// Test Case: Display Writes Subsequent Frames Without Header
static void test_Display_SubsequentFrames_NoHeader(void **state) {
    (void)state;
    
    // Setup vout_display_t and system structure
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    sys.is_yuv4mpeg2 = true;
    sys.is_first = true; // Header already written
    FILE mock_file;
    sys.f = &mock_file;
    vd.sys = &sys;
    
    // Assign window operations
    vd.ops = &ops;
    
    // Define video format
    vd.fmtp.i_chroma = VLC_CODEC_I420;
    vd.fmtp.i_visible_width = 640;
    vd.fmtp.i_visible_height = 480;
    vd.fmtp.i_frame_rate = 25;
    vd.fmtp.i_frame_rate_base = 1;
    vd.fmtp.i_sar_num = 1;
    vd.fmtp.i_sar_den = 1;
    
    // Mock fwrite for frame data
    expect_any(fwrite, ptr);
    expect_any(fwrite, size);
    expect_any(fwrite, nmemb);
    expect_any(fwrite, stream);
    will_return(fwrite, 0); // Simulate successful fwrite
    
    // Mock fflush to return 0 (success)
    expect_any(fflush, stream);
    will_return(fflush, 0);
    
    // Create a mock picture
    picture_t picture;
    memset(&picture, 0, sizeof(picture_t));
    picture.b_progressive = true;
    picture.b_top_field_first = true;
    picture.i_planes = 3;
    
    // Initialize plane data
    for(int i = 0; i < picture.i_planes; i++) {
        picture.p[i].p_pixels = malloc(100); // Dummy data
        picture.p[i].i_visible_pitch = 100;
        picture.p[i].i_pitch = 100;
        picture.p[i].i_visible_lines = 1;
    }
    
    // Call Display function
    Display(&vd, &picture);
    
    // Assertions
    assert_true(sys.is_first);
    
    // Cleanup
    for(int i = 0; i < picture.i_planes; i++) {
        free(picture.p[i].p_pixels);
    }
}

// Test Case: Display Handles Non-Progressive Frame Gracefully
static void test_Display_NonProgressiveFrame(void **state) {
    (void)state;
    
    // Setup vout_display_t and system structure
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    sys.is_yuv4mpeg2 = false;
    sys.is_first = false;
    FILE mock_file;
    sys.f = &mock_file;
    vd.sys = &sys;
    
    // Assign window operations
    vd.ops = &ops;
    
    // Define video format
    vd.fmtp.i_chroma = VLC_CODEC_I420;
    vd.fmtp.i_visible_width = 640;
    vd.fmtp.i_visible_height = 480;
    vd.fmtp.i_frame_rate = 25;
    vd.fmtp.i_frame_rate_base = 1;
    vd.fmtp.i_sar_num = 1;
    vd.fmtp.i_sar_den = 1;
    
    // Mock fwrite for header and frame data
    // Header is not written since YUV4MPEG2 is disabled
    expect_any(fwrite, ptr);
    expect_any(fwrite, size);
    expect_any(fwrite, nmemb);
    expect_any(fwrite, stream);
    will_return(fwrite, 0); // Simulate successful fwrite
    
    // Mock fflush to return 0 (success)
    expect_any(fflush, stream);
    will_return(fflush, 0);
    
    // Create a mock picture with non-progressive flag
    picture_t picture;
    memset(&picture, 0, sizeof(picture_t));
    picture.b_progressive = false; // Non-progressive
    picture.b_top_field_first = false;
    picture.i_planes = 3;
    
    // Initialize plane data
    for(int i = 0; i < picture.i_planes; i++) {
        picture.p[i].p_pixels = malloc(100); // Dummy data
        picture.p[i].i_visible_pitch = 100;
        picture.p[i].i_pitch = 100;
        picture.p[i].i_visible_lines = 1;
    }
    
    // Call Display function
    Display(&vd, &picture);
    
    // Assertions
    assert_true(sys.is_first);
    
    // Cleanup
    for(int i = 0; i < picture.i_planes; i++) {
        free(picture.p[i].p_pixels);
    }
}

// Test Case: Display Handles fwrite Failure Gracefully
static void test_Display_fwrite_Failure(void **state) {
    (void)state;
    
    // Setup vout_display_t and system structure
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    sys.is_yuv4mpeg2 = false;
    sys.is_first = false;
    FILE mock_file;
    sys.f = &mock_file;
    vd.sys = &sys;
    
    // Assign window operations
    vd.ops = &ops;
    
    // Define video format
    vd.fmtp.i_chroma = VLC_CODEC_I420;
    vd.fmtp.i_visible_width = 640;
    vd.fmtp.i_visible_height = 480;
    vd.fmtp.i_frame_rate = 25;
    vd.fmtp.i_frame_rate_base = 1;
    vd.fmtp.i_sar_num = 1;
    vd.fmtp.i_sar_den = 1;
    
    // Mock fwrite to fail on first write
    expect_any(fwrite, ptr);
    expect_any(fwrite, size);
    expect_any(fwrite, nmemb);
    expect_any(fwrite, stream);
    will_return(fwrite, 50); // Simulate partial fwrite
    
    // Mock fwrite to succeed on subsequent writes
    expect_any(fwrite, ptr);
    expect_any(fwrite, size);
    expect_any(fwrite, nmemb);
    expect_any(fwrite, stream);
    will_return(fwrite, 100); // Simulate successful fwrite
    
    // Mock fflush to return 0 (success)
    expect_any(fflush, stream);
    will_return(fflush, 0);
    
    // Create a mock picture
    picture_t picture;
    memset(&picture, 0, sizeof(picture_t));
    picture.b_progressive = true;
    picture.b_top_field_first = true;
    picture.i_planes = 3;
    
    // Initialize plane data
    for(int i = 0; i < picture.i_planes; i++) {
        picture.p[i].p_pixels = malloc(100); // Dummy data
        picture.p[i].i_visible_pitch = 100;
        picture.p[i].i_pitch = 100;
        picture.p[i].i_visible_lines = 1;
    }
    
    // Call Display function
    Display(&vd, &picture);
    
    // Assertions
    // Even if fwrite partially fails, yuv.c does not handle it, so no action taken
    // The test ensures that fwrite was called correctly
    assert_true(function_has_been_called());
    
    // Cleanup
    for(int i = 0; i < picture.i_planes; i++) {
        free(picture.p[i].p_pixels);
    }
}

// Test Case: Close Successfully Closes File and Frees Resources
static void test_Close_Success(void **state) {
    (void)state;
    
    // Setup vout_display_t and system structure
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    FILE mock_file;
    sys.f = &mock_file;
    vd.sys = &sys;
    
    // Assign window operations
    vd.ops = &ops;
    
    // Expect fclose to be called with the correct FILE pointer
    expect_any(fclose, stream);
    will_return(fclose, 0); // Simulate successful fclose
    
    // Expect free to be called with sys pointer
    expect_value(malloc_wrapper, size, sizeof(vout_display_sys_t)); // Not directly applicable
    // Instead, we will mock free_wrapper
    expect_value(free_wrapper, ptr, &sys);
    will_be_called(free_wrapper);
    
    // Call Close function
    Close(&vd);
    
    // Assertions
    assert_true(function_has_been_called());
}

// Test Case: Control Handles Supported Queries Successfully
static void test_Control_SupportedQueries(void **state) {
    (void)state;
    
    // Setup vout_display_t and system structure
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    vd.sys = &sys;
    
    // Assign window operations
    vd.ops = &ops;
    
    // Define supported queries
    int supported_queries[] = {
        VOUT_DISPLAY_CHANGE_DISPLAY_SIZE,
        VOUT_DISPLAY_CHANGE_SOURCE_ASPECT,
        VOUT_DISPLAY_CHANGE_SOURCE_CROP,
        VOUT_DISPLAY_CHANGE_SOURCE_PLACE
    };
    
    // Iterate over supported queries and test
    for(int i = 0; i < sizeof(supported_queries)/sizeof(supported_queries[0]); i++) {
        int query = supported_queries[i];
        int ret = Control(&vd, query);
        assert_int_equal(ret, VLC_SUCCESS);
    }
}

// Test Case: Control Handles Unsupported Queries Gracefully
static void test_Control_UnsupportedQueries(void **state) {
    (void)state;
    
    // Setup vout_display_t and system structure
    vout_display_t vd;
    memset(&vd, 0, sizeof(vout_display_t));
    
    vout_display_sys_t sys;
    memset(&sys, 0, sizeof(vout_display_sys_t));
    vd.sys = &sys;
    
    // Assign window operations
    vd.ops = &ops;
    
    // Define unsupported queries
    int unsupported_queries[] = {
        9999, // Undefined query
        -1,   // Negative query
        0     // Typically not used
    };
    
    // Iterate over unsupported queries and test
    for(int i = 0; i < sizeof(unsupported_queries)/sizeof(unsupported_queries[0]); i++) {
        int query = unsupported_queries[i];
        int ret = Control(&vd, query);
        assert_int_equal(ret, VLC_EGENERIC);
    }
}

// --------------------------------------------
// Main Function to Run Tests
// --------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_Open_Success_WithYUV4MPEG2),
        cmocka_unit_test(test_Open_Failure_MemoryAllocation),
        cmocka_unit_test(test_Open_Failure_FileOpening),
        cmocka_unit_test(test_Open_Failure_UnsupportedChroma_YUV4MPEG2),
        cmocka_unit_test(test_Display_FirstFrame_WithYUV4MPEG2),
        cmocka_unit_test(test_Display_SubsequentFrames_NoHeader),
        cmocka_unit_test(test_Display_NonProgressiveFrame),
        cmocka_unit_test(test_Display_fwrite_Failure),
        cmocka_unit_test(test_Close_Success),
        cmocka_unit_test(test_Control_SupportedQueries),
        cmocka_unit_test(test_Control_UnsupportedQueries),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
