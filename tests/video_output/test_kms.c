// test_kms.c

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <drm_fourcc.h>
#include <errno.h>
#include <unistd.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

// Include the kms.c source here
// For testing purposes, it's recommended to have a header (kms.h) with necessary declarations.
// However, for this example, we'll assume kms.c can be included directly.
// Adjust the path as necessary based on your project structure.
#include "kms.c" // Adjust the path as necessary

// --------------------------------------------
// Mock Definitions for External Dependencies
// --------------------------------------------

// Define minimal structures for testing
typedef struct vlc_object_t vlc_object_t;
typedef struct vlc_window_cfg_t vlc_window_cfg_t;

typedef struct vlc_window {
    struct vout_window_sys_t *sys;
    const struct vlc_window_operations *ops;
    struct vlc_window_cfg_t *cfg;
    // Additional fields as needed
} vlc_window_t;

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

// Mock var_InheritString
char* var_InheritString(vlc_object_t* obj, const char* varname) {
    check_expected(obj);
    check_expected(varname);
    return (char*)mock();
}

// Mock var_SetString
void var_SetString(vlc_object_t* obj, const char* varname, const char* value) {
    function_called();
    check_expected(obj);
    check_expected(varname);
    check_expected(value);
}

// Mock var_Create
void var_Create(vlc_object_t* obj, const char* varname, int type) {
    function_called();
    check_expected(obj);
    check_expected(varname);
    check_expected(type);
}

// Mock var_SetAddress
void var_SetAddress(vlc_object_t* obj, const char* varname, void* value) {
    function_called();
    check_expected(obj);
    check_expected(varname);
    check_expected(value);
}

// Mock var_GetAddress
void* var_GetAddress(vlc_object_t* obj, const char* varname) {
    function_called();
    check_expected(obj);
    check_expected(varname);
    return (void*)mock();
}

// Mock memory functions
void* __wrap_malloc(size_t size) {
    check_expected(size);
    return (void*)mock();
}

void __wrap_free(void* ptr) {
    function_called();
    check_expected(ptr);
}

// Mock DRM functions
drmModeRes* drmModeGetResources(int fd) {
    check_expected(fd);
    return (drmModeRes*)mock();
}

drmModeConnector* drmModeGetConnector(int fd, uint32_t connector_id) {
    check_expected(fd);
    check_expected(connector_id);
    return (drmModeConnector*)mock();
}

drmModeEncoder* drmModeGetEncoder(int fd, uint32_t encoder_id) {
    check_expected(fd);
    check_expected(encoder_id);
    return (drmModeEncoder*)mock();
}

void drmModeFreeConnector(drmModeConnector* conn) {
    function_called();
    check_expected_ptr(conn);
}

void drmModeFreeEncoder(drmModeEncoder* enc) {
    function_called();
    check_expected_ptr(enc);
}

drmModeCrtc* drmModeGetCrtc(int fd, uint32_t crtc_id) {
    check_expected(fd);
    check_expected(crtc_id);
    return (drmModeCrtc*)mock();
}

int drmIoctl(int fd, unsigned long request, void *arg) {
    check_expected(fd);
    check_expected(request);
    check_expected_ptr(arg);
    return mock_type(int);
}

int drmModeSetCrtc(int fd, uint32_t crtc_id, uint32_t buffer_id,
                  int x, int y, uint32_t *connectors, int count,
                  drmModeModeInfo *mode) {
    check_expected(fd);
    check_expected(crtc_id);
    check_expected(buffer_id);
    check_expected(x);
    check_expected(y);
    check_expected_ptr(connectors);
    check_expected(count);
    check_expected_ptr(mode);
    return mock_type(int);
}

int drmModeAddFB(int fd, uint32_t width, uint32_t height, uint32_t depth,
               uint32_t bpp, uint32_t pitch, uint32_t bo_handle,
               uint32_t *fb_id) {
    check_expected(fd);
    check_expected(width);
    check_expected(height);
    check_expected(depth);
    check_expected(bpp);
    check_expected(pitch);
    check_expected(bo_handle);
    check_expected_ptr(fb_id);
    *fb_id = (uint32_t)mock_type(uint32_t);
    return mock_type(int);
}

int drmModeRmFB(int fd, uint32_t fb_id) {
    check_expected(fd);
    check_expected(fb_id);
    return mock_type(int);
}

drmModeCrtc* drmModeGetCrtc(int fd, uint32_t crtc_id) {
    check_expected(fd);
    check_expected(crtc_id);
    return (drmModeCrtc*)mock();
}

void drmModeFreeCrtc(drmModeCrtc* crtc) {
    function_called();
    check_expected_ptr(crtc);
}

drmModeFB* drmModeGetFB(int fd, uint32_t fb_id) {
    check_expected(fd);
    check_expected(fb_id);
    return (drmModeFB*)mock();
}

drmModeRes* drmModeGetResources(int fd) {
    check_expected(fd);
    return (drmModeRes*)mock();
}

drmVersionPtr drmGetVersion(int fd) {
    check_expected(fd);
    return (drmVersionPtr)mock();
}

void drmFreeVersion(drmVersionPtr version) {
    function_called();
    check_expected_ptr(version);
}

int drmSetClientCap(int fd, uint64_t capability, uint64_t value) {
    check_expected(fd);
    check_expected(capability);
    check_expected(value);
    return mock_type(int);
}

int drmDropMaster(int fd) {
    check_expected(fd);
    return mock_type(int);
}

int vlc_open(const char *pathname, int flags) {
    check_expected_ptr(pathname);
    check_expected(flags);
    return mock_type(int);
}

int vlc_close(int fd) {
    check_expected(fd);
    return mock_type(int);
}

// Define VLC structures and constants
typedef struct vout_window_sys_t {
    /* modeset information */
    uint32_t crtc;

    /* DRM file descriptor */
    int drm_fd;

    /*
     * buffer information
     */
    uint32_t width;
    uint32_t height;

    drmModeRes *modeRes;
    drmModeModeInfo *mode;
    drmModeConnector *conn;
    uint32_t connector;
    drmModeCrtc *saved_crtc;

    uint32_t framebuffer;
    uint32_t main_buffer;
} vout_window_sys_t;

// Define constants
#define VLC_SUCCESS 0
#define VLC_ENOMEM -12
#define VLC_EGENERIC -1

#define DRM_DIR_NAME "drm"
#define DRM_PRIMARY_MINOR_NAME "card"

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

// Define drm_client_capabilities if not defined
#ifndef DRM_CLIENT_CAP_UNIVERSAL_PLANES
#define DRM_CLIENT_CAP_UNIVERSAL_PLANES 0x1
#endif

// Define KMS_DEVICE_VAR if not defined
#ifndef KMS_DEVICE_VAR
#define KMS_DEVICE_VAR "kms-device"
#endif

// Define VOUT_DISPLAY_CHANGE_DISPLAY_SIZE and other control queries
#define VOUT_DISPLAY_CHANGE_DISPLAY_SIZE 1
#define VOUT_DISPLAY_CHANGE_SOURCE_ASPECT 2
#define VOUT_DISPLAY_CHANGE_SOURCE_CROP 3
#define VOUT_DISPLAY_CHANGE_SOURCE_PLACE 4

// Define window operations structure
typedef struct vlc_window_operations {
    void (*destroy)(vlc_window_t *wnd);
    int (*enable)(vlc_window_t *wnd, const vlc_window_cfg_t *cfg);
    void (*disable)(vlc_window_t *wnd);
} vlc_window_operations_t;

// Mock functions used in kms.c
void vlc_window_ReportOutputDevice(vlc_window_t *wnd, const char *name, const char *desc) {
    function_called();
    check_expected_ptr(wnd);
    check_expected(name);
    check_expected(desc);
}

void vlc_window_ReportSize(vlc_window_t *wnd, uint32_t width, uint32_t height) {
    function_called();
    check_expected_ptr(wnd);
    check_expected(width);
    check_expected(height);
}

// --------------------------------------------
// Example Test Cases
// --------------------------------------------

// Test Case: OpenWindow Successfully Opens DRM Device
static void test_kms_OpenWindow_Success(void **state) {
    (void)state;

    // Setup vlc_window_t and related structures
    vlc_window_t wnd;
    vout_window_sys_t *sys;
    memset(&wnd, 0, sizeof(vlc_window_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_window_sys_t));
    void* mock_sys = malloc(sizeof(vout_window_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_window_sys_t));

    // Mock var_InheritString for "kms-device"
    expect_any(var_InheritString, obj);
    expect_string(var_InheritString, varname, KMS_DEVICE_VAR);
    will_return(var_InheritString, strdup("/dev/dri/card0")); // Mock DRM device path

    // Mock vlc_open to return a valid file descriptor
    expect_string(vlc_open, pathname, "/dev/dri/card0");
    expect_value(vlc_open, flags, O_RDWR);
    will_return(vlc_open, 4); // Mock file descriptor

    // Mock drmGetVersion to return a valid version
    drmVersion version = {
        .version_major = 2,
        .version_minor = 4,
        .version_patchlevel = 0,
        .name = "i915",
        .date = "2021-01-01",
        .desc = "Intel i915 Driver"
    };
    drmVersionPtr mock_version = &version;
    expect_value(drmGetVersion, fd, 4);
    will_return(drmGetVersion, mock_version);

    // Mock drmFreeVersion
    expect_ptr(drmFreeVersion, version, mock_version);
    // drmFreeVersion is a void function, just ensure it's called

    // Mock drmSetClientCap to succeed
    expect_value(drmSetClientCap, fd, 4);
    expect_value(drmSetClientCap, capability, DRM_CLIENT_CAP_UNIVERSAL_PLANES);
    expect_value(drmSetClientCap, value, 1);
    will_return(drmSetClientCap, 0); // Success

    // Mock drmModeGetResources to return valid resources
    drmModeRes mock_res = {
        .count_connectors = 1,
        .count_crtcs = 1,
        .connectors = (uint32_t[]){101},
        .crtcs = (uint32_t[]){201},
        .encoders = (uint32_t[]){301},
        .count_encoders = 1,
        .min_width = 0,
        .max_width = 0,
        .min_height = 0,
        .max_height = 0
    };
    drmModeRes *mock_modeRes = &mock_res;
    expect_value(drmModeGetResources, fd, 4);
    will_return(drmModeGetResources, mock_modeRes);

    // Mock drmModeGetConnector to return a valid connector
    drmModeConnector mock_conn = {
        .connector_id = 101,
        .connector_type = DRM_MODE_CONNECTOR_HDMIA,
        .connector_type_id = 1,
        .connection = DRM_MODE_CONNECTED,
        .count_modes = 1,
        .modes = (drmModeModeInfo[]){{
            .hdisplay = 1920,
            .vdisplay = 1080,
            // Additional mode info as needed
        }},
        .count_encoders = 1,
        .encoders = (uint32_t[]){301},
        .count_props = 0,
        .props = NULL,
        .prop_values = NULL
    };
    drmModeConnector *mock_connector = &mock_conn;
    expect_value(drmModeGetConnector, fd, 4);
    expect_value(drmModeGetConnector, connector_id, 101);
    will_return(drmModeGetConnector, mock_connector);

    // Mock drmModeSetCrtc to succeed
    expect_value(drmModeSetCrtc, fd, 4);
    expect_value(drmModeSetCrtc, crtc_id, 201);
    expect_value(drmModeSetCrtc, buffer_id, 0); // Assuming buffer_id is 0 initially
    expect_value(drmModeSetCrtc, x, 0);
    expect_value(drmModeSetCrtc, y, 0);
    uint32_t connectors[] = {101};
    expect_memory(drmModeSetCrtc, connectors, connectors, sizeof(connectors));
    expect_value(drmModeSetCrtc, count, 1);
    drmModeModeInfo mock_mode = {
        .hdisplay = 1920,
        .vdisplay = 1080,
        // Additional mode info as needed
    };
    expect_memory(drmModeSetCrtc, mode, &mock_mode, sizeof(mock_mode));
    will_return(drmModeSetCrtc, 0); // Success

    // Mock vlc_window_ReportSize
    expect_any(vlc_window_ReportSize, wnd);
    expect_any(vlc_window_ReportSize, width);
    expect_any(vlc_window_ReportSize, height);
    will_return(vlc_window_ReportSize, 0); // No return value needed

    // Mock vlc_window_ReportOutputDevice
    expect_any(vlc_window_ReportOutputDevice, wnd);
    expect_any(vlc_window_ReportOutputDevice, name);
    expect_any(vlc_window_ReportOutputDevice, desc);
    will_return(vlc_window_ReportOutputDevice, 0); // No return value needed

    // Call OpenWindow function
    int ret = OpenWindow(&wnd);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    assert_non_null(wnd.sys);
    sys = wnd.sys;
    assert_int_equal(sys->drm_fd, 4);
    assert_ptr_equal(sys->modeRes, mock_modeRes);
    assert_ptr_equal(sys->conn, mock_connector);
    assert_int_equal(sys->connector, 101);
    assert_int_equal(sys->crtc, 201);
    assert_int_equal(sys->width, 1920);
    assert_int_equal(sys->height, 1080);
    assert_ptr_equal(sys->mode, &mock_conn.modes[0]);

    // Cleanup
    wnd.ops->destroy(&wnd);
    free(mock_sys);
    free(var_InheritString(NULL, KMS_DEVICE_VAR)); // Free duplicated string
}

// Test Case: OpenWindow Fails to Open DRM Device
static void test_kms_OpenWindow_Failure_OpenDRM(void **state) {
    (void)state;

    // Setup vlc_window_t and related structures
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_window_sys_t));
    void* mock_sys = malloc(sizeof(vout_window_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_window_sys_t));

    // Mock var_InheritString for "kms-device"
    expect_any(var_InheritString, obj);
    expect_string(var_InheritString, varname, KMS_DEVICE_VAR);
    will_return(var_InheritString, strdup("/dev/dri/card0")); // Mock DRM device path

    // Mock vlc_open to fail
    expect_string(vlc_open, pathname, "/dev/dri/card0");
    expect_value(vlc_open, flags, O_RDWR);
    will_return(vlc_open, -1); // Failure to open DRM device

    // Call OpenWindow function
    int ret = OpenWindow(&wnd);

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
    assert_non_null(wnd.sys); // sys is allocated even on failure

    // Cleanup
    free(wnd.sys);
    free(var_InheritString(NULL, KMS_DEVICE_VAR)); // Free duplicated string
}

// Test Case: OpenWindow Fails Due to Missing DRM Resources
static void test_kms_OpenWindow_Failure_MissingResources(void **state) {
    (void)state;

    // Setup vlc_window_t and related structures
    vlc_window_t wnd;
    vout_window_sys_t *sys;
    memset(&wnd, 0, sizeof(vlc_window_t));

    // Mock memory allocation for sys
    expect_value(__wrap_malloc, size, sizeof(vout_window_sys_t));
    void* mock_sys = malloc(sizeof(vout_window_sys_t));
    will_return(__wrap_malloc, mock_sys);
    memset(mock_sys, 0, sizeof(vout_window_sys_t));

    // Mock var_InheritString for "kms-device"
    expect_any(var_InheritString, obj);
    expect_string(var_InheritString, varname, KMS_DEVICE_VAR);
    will_return(var_InheritString, strdup("/dev/dri/card0")); // Mock DRM device path

    // Mock vlc_open to return a valid file descriptor
    expect_string(vlc_open, pathname, "/dev/dri/card0");
    expect_value(vlc_open, flags, O_RDWR);
    will_return(vlc_open, 4); // Mock file descriptor

    // Mock drmGetVersion to return a valid version
    drmVersion version = {
        .version_major = 2,
        .version_minor = 4,
        .version_patchlevel = 0,
        .name = "i915",
        .date = "2021-01-01",
        .desc = "Intel i915 Driver"
    };
    drmVersionPtr mock_version = &version;
    expect_value(drmGetVersion, fd, 4);
    will_return(drmGetVersion, mock_version);

    // Mock drmFreeVersion
    expect_ptr(drmFreeVersion, version, mock_version);

    // Mock drmSetClientCap to succeed
    expect_value(drmSetClientCap, fd, 4);
    expect_value(drmSetClientCap, capability, DRM_CLIENT_CAP_UNIVERSAL_PLANES);
    expect_value(drmSetClientCap, value, 1);
    will_return(drmSetClientCap, 0); // Success

    // Mock drmModeGetResources to return NULL (failure)
    expect_value(drmModeGetResources, fd, 4);
    will_return(drmModeGetResources, NULL);

    // Expect drmDropMaster and vlc_close to be called during error handling
    expect_value(drmDropMaster, fd, 4);
    will_return(drmDropMaster, 0); // Success

    expect_value(vlc_close, fd, 4);
    will_return(vlc_close, 0); // Success

    // Call OpenWindow function
    int ret = OpenWindow(&wnd);

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
    assert_non_null(wnd.sys);

    // Cleanup
    free(wnd.sys);
    free(var_InheritString(NULL, KMS_DEVICE_VAR)); // Free duplicated string
}

// Test Case: WindowEnable Successfully Sets Up DRM Resources
static void test_kms_WindowEnable_Success(void **state) {
    (void)state;

    // Setup vlc_window_t and related structures
    vlc_window_t wnd;
    vout_window_sys_t sys;
    memset(&wnd, 0, sizeof(vlc_window_t));
    memset(&sys, 0, sizeof(vout_window_sys_t));

    wnd.sys = &sys;

    // Assign mock DRM file descriptor
    sys.drm_fd = 4;

    // Mock var_InheritString for "kms-connector" if needed
    // For simplicity, assume it's not set (NULL)

    // Mock drmModeGetResources to return valid resources
    drmModeRes mock_res = {
        .count_connectors = 1,
        .count_crtcs = 1,
        .connectors = (uint32_t[]){101},
        .crtcs = (uint32_t[]){201},
        .encoders = (uint32_t[]){301},
        .count_encoders = 1,
        .min_width = 0,
        .max_width = 0,
        .min_height = 0,
        .max_height = 0
    };
    drmModeRes *mock_modeRes = &mock_res;
    expect_value(drmModeGetResources, fd, 4);
    will_return(drmModeGetResources, mock_modeRes);

    // Mock drmModeGetConnector to return a valid connector
    drmModeConnector mock_conn = {
        .connector_id = 101,
        .connector_type = DRM_MODE_CONNECTOR_HDMIA,
        .connector_type_id = 1,
        .connection = DRM_MODE_CONNECTED,
        .count_modes = 1,
        .modes = (drmModeModeInfo[]){{
            .hdisplay = 1920,
            .vdisplay = 1080,
            // Additional mode info as needed
        }},
        .count_encoders = 1,
        .encoders = (uint32_t[]){301},
        .count_props = 0,
        .props = NULL,
        .prop_values = NULL
    };
    drmModeConnector *mock_connector = &mock_conn;
    expect_value(drmModeGetConnector, fd, 4);
    expect_value(drmModeGetConnector, connector_id, 101);
    will_return(drmModeGetConnector, mock_connector);

    // Mock drmModeSetCrtc to succeed
    expect_value(drmModeSetCrtc, fd, 4);
    expect_value(drmModeSetCrtc, crtc_id, 201);
    expect_value(drmModeSetCrtc, buffer_id, 0); // Assuming buffer_id is 0 initially
    expect_value(drmModeSetCrtc, x, 0);
    expect_value(drmModeSetCrtc, y, 0);
    uint32_t connectors[] = {101};
    expect_memory(drmModeSetCrtc, connectors, connectors, sizeof(connectors));
    expect_value(drmModeSetCrtc, count, 1);
    drmModeModeInfo mock_mode = {
        .hdisplay = 1920,
        .vdisplay = 1080,
        // Additional mode info as needed
    };
    expect_memory(drmModeSetCrtc, mode, &mock_mode, sizeof(mock_mode));
    will_return(drmModeSetCrtc, 0); // Success

    // Mock drmModeAddFB to succeed and return framebuffer ID 501
    expect_value(drmModeAddFB, fd, 4);
    expect_value(drmModeAddFB, width, 1920);
    expect_value(drmModeAddFB, height, 1080);
    expect_value(drmModeAddFB, depth, 24);
    expect_value(drmModeAddFB, bpp, 32);
    expect_value(drmModeAddFB, pitch, 9216); // 1920 * 32 / 8
    expect_value(drmModeAddFB, bo_handle, 601);
    expect_any_ptr(drmModeAddFB, fb_id);
    will_return(drmModeAddFB, 501); // framebuffer ID

    // Mock drmIoctl for DRM_IOCTL_MODE_CREATE_DUMB to succeed
    struct drm_mode_create_dumb mock_create_dumb = {
        .width = 1920,
        .height = 1080,
        .bpp = 32,
        .pitch = 9216,
        .size = 1920 * 1080 * 4,
        .handle = 601
    };
    expect_value(drmIoctl, fd, 4);
    expect_value(drmIoctl, request, DRM_IOCTL_MODE_CREATE_DUMB);
    expect_memory(drmIoctl, arg, &mock_create_dumb, sizeof(mock_create_dumb));
    will_return(drmIoctl, 0); // Success

    // Mock drmModeAddFB to return framebuffer ID 501
    // Already handled above

    // Mock vlc_window_ReportSize
    expect_any(vlc_window_ReportSize, wnd);
    expect_any(vlc_window_ReportSize, width);
    expect_any(vlc_window_ReportSize, height);
    will_return(vlc_window_ReportSize, 0); // No return value needed

    // Mock vlc_window_ReportOutputDevice
    expect_any(vlc_window_ReportOutputDevice, wnd);
    expect_any(vlc_window_ReportOutputDevice, name);
    expect_any(vlc_window_ReportOutputDevice, desc);
    will_return(vlc_window_ReportOutputDevice, 0); // No return value needed

    // Call WindowEnable function
    const vlc_window_cfg_t mock_cfg = { /* Initialize as needed */ };
    int ret = wnd.ops->enable(&wnd, &mock_cfg);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
    assert_int_equal(sys.crtc, 201);
    assert_int_equal(sys.framebuffer, 501);
    assert_int_equal(sys.main_buffer, 601);
    assert_int_equal(sys.width, 1920);
    assert_int_equal(sys.height, 1080);
    assert_ptr_equal(sys.mode, &mock_conn.modes[0]);

    // Cleanup
    // Assuming that WindowDisable and WindowClose would be called subsequently
}

// Test Case: WindowEnable Fails Due to drmModeSetCrtc Failure
static void test_kms_WindowEnable_Failure_SetCrtc(void **state) {
    (void)state;

    // Setup vlc_window_t and related structures
    vlc_window_t wnd;
    vout_window_sys_t *sys;
    memset(&wnd, 0, sizeof(vlc_window_t));
    memset(&sys, 0, sizeof(vout_window_sys_t));

    wnd.sys = &sys;

    // Assign mock DRM file descriptor
    sys.drm_fd = 4;

    // Mock drmModeGetResources to return valid resources
    drmModeRes mock_res = {
        .count_connectors = 1,
        .count_crtcs = 1,
        .connectors = (uint32_t[]){101},
        .crtcs = (uint32_t[]){201},
        .encoders = (uint32_t[]){301},
        .count_encoders = 1,
        .min_width = 0,
        .max_width = 0,
        .min_height = 0,
        .max_height = 0
    };
    drmModeRes *mock_modeRes = &mock_res;
    expect_value(drmModeGetResources, fd, 4);
    will_return(drmModeGetResources, mock_modeRes);

    // Mock drmModeGetConnector to return a valid connector
    drmModeConnector mock_conn = {
        .connector_id = 101,
        .connector_type = DRM_MODE_CONNECTOR_HDMIA,
        .connector_type_id = 1,
        .connection = DRM_MODE_CONNECTED,
        .count_modes = 1,
        .modes = (drmModeModeInfo[]){{
            .hdisplay = 1920,
            .vdisplay = 1080,
            // Additional mode info as needed
        }},
        .count_encoders = 1,
        .encoders = (uint32_t[]){301},
        .count_props = 0,
        .props = NULL,
        .prop_values = NULL
    };
    drmModeConnector *mock_connector = &mock_conn;
    expect_value(drmModeGetConnector, fd, 4);
    expect_value(drmModeGetConnector, connector_id, 101);
    will_return(drmModeGetConnector, mock_connector);

    // Mock drmModeSetCrtc to fail
    expect_value(drmModeSetCrtc, fd, 4);
    expect_value(drmModeSetCrtc, crtc_id, 201);
    expect_value(drmModeSetCrtc, buffer_id, 0);
    expect_value(drmModeSetCrtc, x, 0);
    expect_value(drmModeSetCrtc, y, 0);
    uint32_t connectors[] = {101};
    expect_memory(drmModeSetCrtc, connectors, connectors, sizeof(connectors));
    expect_value(drmModeSetCrtc, count, 1);
    drmModeModeInfo mock_mode = {
        .hdisplay = 1920,
        .vdisplay = 1080,
        // Additional mode info as needed
    };
    expect_memory(drmModeSetCrtc, mode, &mock_mode, sizeof(mock_mode));
    will_return(drmModeSetCrtc, -1); // Failure

    // Expect drmModeRmFB and drmIoctl for destroying dumb buffer to be called
    expect_value(drmModeRmFB, fd, 4);
    expect_value(drmModeRmFB, fb_id, 0); // buffer_id was 0
    will_return(drmModeRmFB, 0); // Success

    struct drm_mode_destroy_dumb mock_destroy_dumb = {
        .handle = 601
    };
    expect_value(drmIoctl, fd, 4);
    expect_value(drmIoctl, request, DRM_IOCTL_MODE_DESTROY_DUMB);
    expect_memory(drmIoctl, arg, &mock_destroy_dumb, sizeof(mock_destroy_dumb));
    will_return(drmIoctl, 0); // Success

    // Mock drmModeFreeConnector to be called
    expect_ptr(drmModeFreeConnector, conn, mock_connector);

    // Call WindowEnable function
    const vlc_window_cfg_t mock_cfg = { /* Initialize as needed */ };
    int ret = wnd.ops->enable(&wnd, &mock_cfg);

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
    assert_int_equal(sys->crtc, 0);
    assert_int_equal(sys->framebuffer, 0);
    assert_int_equal(sys->main_buffer, 0);

    // Cleanup
    free(wnd.sys);
}

// Test Case: WindowClose Successfully Cleans Up Resources
static void test_kms_WindowClose_Success(void **state) {
    (void)state;

    // Setup vlc_window_t and related structures
    vlc_window_t wnd;
    vout_window_sys_t *sys;
    memset(&wnd, 0, sizeof(vlc_window_t));
    memset(&sys, 0, sizeof(vout_window_sys_t));

    wnd.sys = &sys;

    // Assign mock DRM file descriptor and other resources
    sys->drm_fd = 4;
    sys->modeRes = (drmModeRes*)0x1;
    sys->conn = (drmModeConnector*)0x2;
    sys->saved_crtc = (drmModeCrtc*)0x3;
    sys->framebuffer = 501;
    sys->main_buffer = 601;

    // Mock drmModeRmFB to succeed
    expect_value(drmModeRmFB, fd, 4);
    expect_value(drmModeRmFB, fb_id, 501);
    will_return(drmModeRmFB, 0); // Success

    // Mock drmIoctl for DRM_IOCTL_MODE_DESTROY_DUMB to succeed
    struct drm_mode_destroy_dumb mock_destroy_dumb = {
        .handle = 601
    };
    expect_value(drmIoctl, fd, 4);
    expect_value(drmIoctl, request, DRM_IOCTL_MODE_DESTROY_DUMB);
    expect_memory(drmIoctl, arg, &mock_destroy_dumb, sizeof(mock_destroy_dumb));
    will_return(drmIoctl, 0); // Success

    // Mock drmModeSetCrtc to restore saved CRTC
    expect_value(drmModeSetCrtc, fd, 4);
    expect_value(drmModeSetCrtc, crtc_id, sys->saved_crtc->crtc_id);
    expect_value(drmModeSetCrtc, buffer_id, sys->saved_crtc->buffer_id);
    expect_value(drmModeSetCrtc, x, sys->saved_crtc->x);
    expect_value(drmModeSetCrtc, y, sys->saved_crtc->y);
    uint32_t connectors[] = {sys->connector};
    expect_memory(drmModeSetCrtc, connectors, connectors, sizeof(connectors));
    expect_value(drmModeSetCrtc, count, 1);
    expect_memory(drmModeSetCrtc, mode, sys->saved_crtc->mode, sizeof(*sys->saved_crtc->mode));
    will_return(drmModeSetCrtc, 0); // Success

    // Mock drmModeFreeCrtc to be called
    expect_ptr(drmModeFreeCrtc, crtc, sys->saved_crtc);
    function_called();

    // Mock drmModeFreeConnector to be called
    expect_ptr(drmModeFreeConnector, conn, sys->conn);
    function_called();

    // Mock drmModeFreeResources to be called
    expect_ptr(drmModeFreeResources, res, sys->modeRes);
    function_called();

    // Mock drmDropMaster and vlc_close to be called
    expect_value(drmDropMaster, fd, 4);
    will_return(drmDropMaster, 0); // Success

    expect_value(vlc_close, fd, 4);
    will_return(vlc_close, 0); // Success

    // Call WindowClose function
    wnd.ops->destroy(&wnd);

    // Assertions
    // Ensure that all cleanup functions were called correctly
}

// Test Case: Control Operation with Unsupported Query
static void test_kms_Control_UnsupportedQuery(void **state) {
    (void)state;

    // Setup vout_window_t
    vlc_window_t wnd;
    vout_window_sys_t sys;
    memset(&wnd, 0, sizeof(vlc_window_t));
    memset(&sys, 0, sizeof(vout_window_sys_t));

    wnd.sys = &sys;

    // Define unsupported query
    int query = 9999; // Assuming 9999 is unsupported

    // Expect msg_Err to be called for unsupported query
    expect_any(msg_Err, obj);
    expect_any(msg_Err, fmt);

    // Call Control function
    int ret = Control(&wnd, query);

    // Assertions
    assert_int_equal(ret, VLC_EGENERIC);
}

// Test Case: Control Operation with Supported Query
static void test_kms_Control_SupportedQuery(void **state) {
    (void)state;

    // Setup vout_window_t
    vlc_window_t wnd;
    vout_window_sys_t sys;
    memset(&wnd, 0, sizeof(vlc_window_t));
    memset(&sys, 0, sizeof(vout_window_sys_t));

    wnd.sys = &sys;

    // Define supported query
    int query = VOUT_DISPLAY_CHANGE_DISPLAY_SIZE;

    // Call Control function
    int ret = Control(&wnd, query);

    // Assertions
    assert_int_equal(ret, VLC_SUCCESS);
}

// Test Case: WindowDisable Successfully Releases DRM Resources
static void test_kms_WindowDisable_Success(void **state) {
    (void)state;

    // Setup vlc_window_t and related structures
    vlc_window_t wnd;
    vout_window_sys_t sys;
    memset(&wnd, 0, sizeof(vlc_window_t));
    memset(&sys, 0, sizeof(vout_window_sys_t));

    wnd.sys = &sys;

    // Assign mock DRM file descriptor and other resources
    sys.drm_fd = 4;
    sys.modeRes = (drmModeRes*)0x1;
    sys.conn = (drmModeConnector*)0x2;
    sys.saved_crtc = (drmModeCrtc*)0x3;
    sys.framebuffer = 501;
    sys.main_buffer = 601;

    // Mock drmModeSetCrtc to restore saved CRTC
    expect_value(drmModeSetCrtc, fd, 4);
    expect_value(drmModeSetCrtc, crtc_id, sys->saved_crtc->crtc_id);
    expect_value(drmModeSetCrtc, buffer_id, sys->saved_crtc->buffer_id);
    expect_value(drmModeSetCrtc, x, sys->saved_crtc->x);
    expect_value(drmModeSetCrtc, y, sys->saved_crtc->y);
    uint32_t connectors[] = {sys->connector};
    expect_memory(drmModeSetCrtc, connectors, connectors, sizeof(connectors));
    expect_value(drmModeSetCrtc, count, 1);
    expect_memory(drmModeSetCrtc, mode, sys->saved_crtc->mode, sizeof(*sys->saved_crtc->mode));
    will_return(drmModeSetCrtc, 0); // Success

    // Mock drmModeFreeCrtc to be called
    expect_ptr(drmModeFreeCrtc, crtc, sys->saved_crtc);
    function_called();

    // Mock drmModeFreeConnector to be called
    expect_ptr(drmModeFreeConnector, conn, sys->conn);
    function_called();

    // Mock drmModeRmFB to be called
    expect_value(drmModeRmFB, fd, 4);
    expect_value(drmModeRmFB, fb_id, 501);
    will_return(drmModeRmFB, 0); // Success

    // Mock drmIoctl for DRM_IOCTL_MODE_DESTROY_DUMB to succeed
    struct drm_mode_destroy_dumb mock_destroy_dumb = {
        .handle = 601
    };
    expect_value(drmIoctl, fd, 4);
    expect_value(drmIoctl, request, DRM_IOCTL_MODE_DESTROY_DUMB);
    expect_memory(drmIoctl, arg, &mock_destroy_dumb, sizeof(mock_destroy_dumb));
    will_return(drmIoctl, 0); // Success

    // Call WindowDisable function
    wnd.ops->disable(&wnd);

    // Assertions
    // Ensure that all disable functions were called correctly
}

// Test Case: Close Without Prior Open (Should Handle Gracefully)
static void test_kms_Close_NoPriorOpen(void **state) {
    (void)state;

    // Setup vlc_window_t without initializing sys
    vlc_window_t wnd;
    memset(&wnd, 0, sizeof(vlc_window_t));

    // Call WindowClose function
    wnd.ops = &window_ops;
    wnd.ops->destroy(&wnd);

    // Assertions
    // Ensure that Close handles NULL sys gracefully
    // No assertions needed, just ensure no crash
}

// --------------------------------------------
// Main Function to Run Tests
// --------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_kms_OpenWindow_Success),
        cmocka_unit_test(test_kms_OpenWindow_Failure_OpenDRM),
        cmocka_unit_test(test_kms_OpenWindow_Failure_MissingResources),
        cmocka_unit_test(test_kms_WindowEnable_Success),
        cmocka_unit_test(test_kms_WindowEnable_Failure_SetCrtc),
        cmocka_unit_test(test_kms_WindowClose_Success),
        cmocka_unit_test(test_kms_Control_UnsupportedQuery),
        cmocka_unit_test(test_kms_Control_SupportedQuery),
        cmocka_unit_test(test_kms_WindowDisable_Success),
        cmocka_unit_test(test_kms_Close_NoPriorOpen),
        // Add more tests as needed
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
