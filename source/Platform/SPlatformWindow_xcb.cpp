//General Include
#include "General/SInclude.h"

//Header Include
#include "SPlatformWindow.h"


#if USE_PLATFORM_XCB

void SVkWindow::InitPlatformWindow()
{
    // create connection to X11 server
    const xcb_setup_t		*	setup = nullptr;
    xcb_screen_iterator_t		iter;
    int							screen = 0;

    m_xcb_connection = xcb_connect(nullptr, &screen);
    if (m_xcb_connection == nullptr) {
        std::cout << "Cannot find a compatible Vulkan ICD.\n";
        exit(-1);
    }

    setup = xcb_get_setup(m_xcb_connection);
    iter = xcb_setup_roots_iterator(setup);
    while (screen-- > 0) {
        xcb_screen_next(&iter);
    }
    m_xcb_screen = iter.data;

    // create window
    VkRect2D dimensions = { { 0, 0 },{ m_sizeX, m_sizeY } };

    assert(dimensions.extent.width > 0);
    assert(dimensions.extent.height > 0);

    uint32_t value_mask, value_list[32];

    m_xcb_window = xcb_generate_id(m_xcb_connection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = m_xcb_screen->black_pixel;
    value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(m_xcb_connection, XCB_COPY_FROM_PARENT, m_xcb_window,
        m_xcb_screen->root, dimensions.offset.x, dimensions.offset.y,
        dimensions.extent.width, dimensions.extent.height, 0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT, m_xcb_screen->root_visual,
        value_mask, value_list);

    /* Magic code that will send notification when window is destroyed */
    xcb_intern_atom_cookie_t cookie =
        xcb_intern_atom(m_xcb_connection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply =
        xcb_intern_atom_reply(m_xcb_connection, cookie, 0);

    xcb_intern_atom_cookie_t cookie2 =
        xcb_intern_atom(m_xcb_connection, 0, 16, "WM_DELETE_WINDOW");
    m_xcb_atom_window_reply =
        xcb_intern_atom_reply(m_xcb_connection, cookie2, 0);

    xcb_change_property(m_xcb_connection, XCB_PROP_MODE_REPLACE, m_xcb_window,
        (*reply).atom, 4, 32, 1,
        &(*m_xcb_atom_window_reply).atom);
    free(reply);

    xcb_map_window(m_xcb_connection, m_xcb_window);

    // Force the x/y coordinates to 100,100 results are identical in consecutive
    // runs
    const uint32_t coords[] = { 100, 100 };
    xcb_configure_window(m_xcb_connection, m_xcb_window,
        XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
    xcb_flush(m_xcb_connection);

    /*
    xcb_generic_event_t *e;
    while( ( e = xcb_wait_for_event( xcb_connection ) ) ) {
    if( ( e->response_type & ~0x80 ) == XCB_EXPOSE )
    break;
    }
    */
}

void SVkWindow::DeInitPlatformWindow()
{
    xcb_destroy_window(m_xcb_connection, m_xcb_window);
    xcb_disconnect(m_xcb_connection);
    m_xcb_window = 0;
    m_xcb_connection = nullptr;
}

void SVkWindow::UpdatePlatformWindow()
{
    auto event = xcb_poll_for_event(m_xcb_connection);

    // if there is no event, event will be NULL
    // need to check for event == NULL to prevent segfault
    if (!event)
        return;

    switch (event->response_type & ~0x80) {
    case XCB_CLIENT_MESSAGE:
        if (((xcb_client_message_event_t*)event)->data.data32[0] == m_xcb_atom_window_reply->atom) {
            Close();
        }
        break;
    default:
        break;
    }
    free(event);
}

#endif