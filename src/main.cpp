#include <flecs_sokol_cimgui.h>

#include "sokol/sokol.h"

#ifndef __APPLE__
#define SOKOL_IMPL
#define SOKOL_IMGUI_IMPL
#endif

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

static struct
{
    sg_pass_action pass_action;
} state;

static void init(void)
{
    sg_setup({.context = sapp_sgcontext()});
    simgui_setup({.max_vertices = 0});

    // initial clear color
    state.pass_action = {
        .colors[0] = {
            .action = SG_ACTION_CLEAR, .value = {0.0f, 0.5f, 1.0f, 1.0}}};
}

static void frame(void)
{
    simgui_new_frame({
        .width      = sapp_width(),
        .height     = sapp_height(),
        .delta_time = sapp_frame_duration(),
        .dpi_scale  = sapp_dpi_scale(),
    });

    /*=== UI CODE STARTS HERE ===*/
    igSetNextWindowPos({10, 10}, ImGuiCond_Once, {0, 0});
    igSetNextWindowSize({400, 100}, ImGuiCond_Once);
    igBegin("Hello Dear ImGui!", 0, ImGuiWindowFlags_None);
    igColorEdit3(
        "Background", &state.pass_action.colors[0].value.r,
        ImGuiColorEditFlags_None
    );
    igEnd();
    /*=== UI CODE ENDS HERE ===*/

    sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
    simgui_render();
    sg_end_pass();
    sg_commit();
}

static void cleanup(void)
{
    simgui_shutdown();
    sg_shutdown();
}

static void event(const sapp_event *ev) { simgui_handle_event(ev); }

static int sokol_run_action(ecs_world_t *world, ecs_app_desc_t *desc)
{
    /* Run app */
    sapp_run({
        .init_cb            = init,
        .frame_cb           = frame,
        .cleanup_cb         = cleanup,
        .event_cb           = event,
        .window_title       = "Hello Flecs + Sokol + Dear ImGui",
        .width              = 800,
        .height             = 600,
        .icon.sokol_default = true,
    });

    return 0;
}

int main(int argc, char *argv[])
{
    flecs::world ecs;
    ecs_app_set_run_action(sokol_run_action);
    ecs.app().run();
    return 0;
}
