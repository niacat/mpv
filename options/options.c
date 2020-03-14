/*
 * This file is part of mpv.
 *
 * mpv is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * mpv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with mpv.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MPLAYER_CFG_MPLAYER_H
#define MPLAYER_CFG_MPLAYER_H

/*
 * config for cfgparser
 */

#include <float.h>
#include <stddef.h>
#include <sys/types.h>
#include <limits.h>
#include <math.h>

#include "config.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "options.h"
#include "m_config.h"
#include "m_option.h"
#include "common/common.h"
#include "stream/stream.h"
#include "video/csputils.h"
#include "video/hwdec.h"
#include "video/image_writer.h"
#include "sub/osd.h"
#include "player/core.h"
#include "player/command.h"
#include "stream/stream.h"

#if HAVE_DRM
#include "video/out/drm_common.h"
#endif

static void print_version(struct mp_log *log)
{
    mp_print_version(log, true);
}

extern const struct m_sub_options tv_params_conf;
extern const struct m_sub_options stream_cdda_conf;
extern const struct m_sub_options stream_dvb_conf;
extern const struct m_sub_options stream_lavf_conf;
extern const struct m_sub_options sws_conf;
extern const struct m_sub_options zimg_conf;
extern const struct m_sub_options drm_conf;
extern const struct m_sub_options demux_rawaudio_conf;
extern const struct m_sub_options demux_rawvideo_conf;
extern const struct m_sub_options demux_lavf_conf;
extern const struct m_sub_options demux_mkv_conf;
extern const struct m_sub_options demux_cue_conf;
extern const struct m_sub_options vd_lavc_conf;
extern const struct m_sub_options ad_lavc_conf;
extern const struct m_sub_options input_config;
extern const struct m_sub_options encode_config;
extern const struct m_sub_options gl_video_conf;
extern const struct m_sub_options ao_alsa_conf;

extern const struct m_sub_options demux_conf;
extern const struct m_sub_options demux_cache_conf;

extern const struct m_obj_list vf_obj_list;
extern const struct m_obj_list af_obj_list;
extern const struct m_obj_list vo_obj_list;

extern const struct m_sub_options ao_conf;

extern const struct m_sub_options opengl_conf;
extern const struct m_sub_options vulkan_conf;
extern const struct m_sub_options spirv_conf;
extern const struct m_sub_options d3d11_conf;
extern const struct m_sub_options d3d11va_conf;
extern const struct m_sub_options angle_conf;
extern const struct m_sub_options cocoa_conf;
extern const struct m_sub_options macos_conf;
extern const struct m_sub_options android_conf;
extern const struct m_sub_options wayland_conf;
extern const struct m_sub_options vaapi_conf;

static const struct m_sub_options screenshot_conf = {
    .opts = image_writer_opts,
    .size = sizeof(struct image_writer_opts),
    .defaults = &image_writer_opts_defaults,
};

#undef OPT_BASE_STRUCT
#define OPT_BASE_STRUCT struct mp_vo_opts

static const m_option_t mp_vo_opt_list[] = {
    OPT_SETTINGSLIST("vo", video_driver_list, 0, &vo_obj_list, ),
    {"taskbar-progress", OPTF_FLAG(taskbar_progress)},
    {"snap-window", OPTF_FLAG(snap_window)},
    {"ontop", OPTF_FLAG(ontop)},
    {"ontop-level", OPTF_CHOICE(ontop_level, ({"window", -1}, {"system", -2})), .min = 0, .max = INT_MAX},
    {"border", OPTF_FLAG(border)},
    {"fit-border", OPTF_FLAG(fit_border)},
    {"on-all-workspaces", OPTF_FLAG(all_workspaces)},
    {"geometry", OPTF_GEOMETRY(geometry)},
    {"autofit", OPTF_SIZE_BOX(autofit)},
    {"autofit-larger", OPTF_SIZE_BOX(autofit_larger)},
    {"autofit-smaller", OPTF_SIZE_BOX(autofit_smaller)},
    {"window-scale", OPTF_DOUBLE(window_scale), .min = 0.001, .max = 100},
    {"window-minimized", OPTF_FLAG(window_minimized)},
    {"window-maximized", OPTF_FLAG(window_maximized)},
    {"force-window-position", OPTF_FLAG(force_window_position)},
    {"x11-name", OPTF_STRING(winname)},
    {"monitoraspect", OPTF_FLOAT(force_monitor_aspect), .min = 0.0, .max = 9.0},
    {"monitorpixelaspect", OPTF_FLOAT(monitor_pixel_aspect), .min = 1.0/32.0, .max = 32.0},
    {"fullscreen", OPTF_BOOL(fullscreen)},
    {"fs", OPTF_ALIAS("fullscreen")},
    {"native-keyrepeat", OPTF_FLAG(native_keyrepeat)},
    {"panscan", OPTF_FLOAT(panscan), .min = 0.0, .max = 1.0},
    {"video-zoom", OPTF_FLOAT(zoom), .min = -20.0, .max = 20.0},
    {"video-pan-x", OPTF_FLOAT(pan_x), .min = -3.0, .max = 3.0},
    {"video-pan-y", OPTF_FLOAT(pan_y), .min = -3.0, .max = 3.0},
    {"video-align-x", OPTF_FLOAT(align_x), .min = -1.0, .max = 1.0},
    {"video-align-y", OPTF_FLOAT(align_y), .min = -1.0, .max = 1.0},
    {"video-margin-ratio-left", OPTF_FLOAT(margin_x[0]), .min = 0.0, .max = 1.0},
    {"video-margin-ratio-right", OPTF_FLOAT(margin_x[1]), .min = 0.0, .max = 1.0},
    {"video-margin-ratio-top", OPTF_FLOAT(margin_y[0]), .min = 0.0, .max = 1.0},
    {"video-margin-ratio-bottom", OPTF_FLOAT(margin_y[1]), .min = 0.0, .max = 1.0},
    {"video-unscaled", OPTF_CHOICE(unscaled,
     ({"no", 0}, {"yes", 1}, {"downscale-big", 2}))},
    {"wid", OPTF_INT64(WinID)},
    {"screen", OPTF_CHOICE(screen_id, ({"default", -1})), .min = 0, .max = 32},
    {"fs-screen", OPTF_CHOICE(fsscreen_id, ({"all", -2}, {"current", -1})),
     .min = 0, .max = 32},
    {"keepaspect", OPTF_FLAG(keepaspect)},
    {"keepaspect-window", OPTF_FLAG(keepaspect_window)},
    {"hidpi-window-scale", OPTF_FLAG(hidpi_window_scale)},
    {"native-fs", OPTF_FLAG(native_fs)},
    {"override-display-fps", OPTF_DOUBLE(override_display_fps), .min = 0, .max = DBL_MAX},
    {"video-timing-offset", OPTF_DOUBLE(timing_offset), .min = 0.0, .max = 1.0},
#if HAVE_X11
    {"x11-netwm", OPTF_CHOICE(x11_netwm, ({"auto", 0}, {"no", -1}, {"yes", 1}))},
    {"x11-bypass-compositor", OPTF_CHOICE(x11_bypass_compositor,
     ({"no", 0}, {"yes", 1}, {"fs-only", 2}, {"never", 3}))},
#endif
#if HAVE_WIN32_DESKTOP
    {"vo-mmcss-profile", OPTF_STRING(mmcss_profile)},
#endif
#if HAVE_DRM
    {"", OPTF_SUBSTRUCT(drm_opts, drm_conf)},
#endif
    {"swapchain-depth", OPTF_INT(swapchain_depth), .min = 1, .max = 8},
    {0}
};

const struct m_sub_options vo_sub_opts = {
    .opts = mp_vo_opt_list,
    .size = sizeof(struct mp_vo_opts),
    .defaults = &(const struct mp_vo_opts){
        .video_driver_list = NULL,
        .monitor_pixel_aspect = 1.0,
        .screen_id = -1,
        .fsscreen_id = -1,
        .panscan = 0.0f,
        .keepaspect = 1,
        .keepaspect_window = 1,
        .hidpi_window_scale = 1,
        .native_fs = 1,
        .taskbar_progress = 1,
        .snap_window = 0,
        .border = 1,
        .fit_border = 1,
        .WinID = -1,
        .window_scale = 1.0,
        .x11_bypass_compositor = 2,
        .mmcss_profile = "Playback",
        .ontop_level = -1,
        .timing_offset = 0.050,
        .swapchain_depth = 3,
    },
};

#undef OPT_BASE_STRUCT
#define OPT_BASE_STRUCT struct mp_sub_filter_opts

const struct m_sub_options mp_sub_filter_opts = {
    .opts = (const struct m_option[]){
        {"sub-filter-sdh", OPTF_FLAG(sub_filter_SDH)},
        {"sub-filter-sdh-harder", OPTF_FLAG(sub_filter_SDH_harder)},
        {"sub-filter-regex-enable", OPTF_FLAG(rf_enable)},
        {"sub-filter-regex", OPTF_STRINGLIST(rf_items)},
        {"sub-filter-regex-warn", OPTF_FLAG(rf_warn)},
        {0}
    },
    .size = sizeof(OPT_BASE_STRUCT),
    .defaults = &(OPT_BASE_STRUCT){
        .rf_enable = 1,
    },
    .change_flags = UPDATE_SUB_FILT,
};

#undef OPT_BASE_STRUCT
#define OPT_BASE_STRUCT struct mp_subtitle_opts

const struct m_sub_options mp_subtitle_sub_opts = {
    .opts = (const struct m_option[]){
        {"sub-delay", OPTF_FLOAT(sub_delay)},
        {"sub-fps", OPTF_FLOAT(sub_fps)},
        {"sub-speed", OPTF_FLOAT(sub_speed)},
        {"sub-visibility", OPTF_FLAG(sub_visibility)},
        {"sub-forced-only", OPTF_FLAG(forced_subs_only)},
        {"stretch-dvd-subs", OPTF_FLAG(stretch_dvd_subs)},
        {"stretch-image-subs-to-screen", OPTF_FLAG(stretch_image_subs)},
        {"image-subs-video-resolution", OPTF_FLAG(image_subs_video_res)},
        {"sub-fix-timing", OPTF_FLAG(sub_fix_timing)},
        {"sub-pos", OPTF_INT(sub_pos), .min = 0, .max = 100},
        {"sub-gauss", OPTF_FLOAT(sub_gauss), .min = 0.0, .max = 3.0},
        {"sub-gray", OPTF_FLAG(sub_gray)},
        {"sub-ass", OPTF_FLAG(ass_enabled)},
        {"sub-scale", OPTF_FLOAT(sub_scale), .min = 0, .max = 100},
        {"sub-ass-line-spacing", OPTF_FLOAT(ass_line_spacing), .min = -1000, .max = 1000},
        {"sub-use-margins", OPTF_FLAG(sub_use_margins)},
        {"sub-ass-force-margins", OPTF_FLAG(ass_use_margins)},
        {"sub-ass-vsfilter-aspect-compat", OPTF_FLAG(ass_vsfilter_aspect_compat)},
        {"sub-ass-vsfilter-color-compat", OPTF_CHOICE(ass_vsfilter_color_compat,
         ({"no", 0}, {"basic", 1}, {"full", 2}, {"force-601", 3}))},
        {"sub-ass-vsfilter-blur-compat", OPTF_FLAG(ass_vsfilter_blur_compat)},
        {"embeddedfonts", OPTF_FLAG(use_embedded_fonts)},
        {"sub-ass-force-style", OPTF_STRINGLIST(ass_force_style_list)},
        {"sub-ass-styles", OPTF_STRING(ass_styles_file), .flags = M_OPT_FILE},
        {"sub-ass-hinting", OPTF_CHOICE(ass_hinting,
         ({"none", 0}, {"light", 1}, {"normal", 2}, {"native", 3}))},
        {"sub-ass-shaper", OPTF_CHOICE(ass_shaper,
         ({"simple", 0}, {"complex", 1}))},
        {"sub-ass-justify", OPTF_FLAG(ass_justify)},
        {"sub-ass-override", OPTF_CHOICE(ass_style_override,
         ({"no", 0}, {"yes", 1}, {"force", 3}, {"scale", 4}, {"strip", 5}))},
        {"sub-scale-by-window", OPTF_FLAG(sub_scale_by_window)},
        {"sub-scale-with-window", OPTF_FLAG(sub_scale_with_window)},
        {"sub-ass-scale-with-window", OPTF_FLAG(ass_scale_with_window)},
        {"sub", OPTF_SUBSTRUCT(sub_style, sub_style_conf)},
        {"sub-clear-on-seek", OPTF_FLAG(sub_clear_on_seek)},
        {"teletext-page", OPTF_INT(teletext_page), .min = 1, .max = 999},
        {0}
    },
    .size = sizeof(OPT_BASE_STRUCT),
    .defaults = &(OPT_BASE_STRUCT){
        .sub_visibility = 1,
        .sub_pos = 100,
        .sub_speed = 1.0,
        .ass_enabled = 1,
        .sub_scale_by_window = 1,
        .ass_use_margins = 0,
        .sub_use_margins = 1,
        .ass_scale_with_window = 0,
        .sub_scale_with_window = 1,
        .teletext_page = 100,
        .sub_scale = 1,
        .ass_vsfilter_aspect_compat = 1,
        .ass_vsfilter_color_compat = 1,
        .ass_vsfilter_blur_compat = 0,
        .ass_style_override = 1,
        .ass_shaper = 1,
        .use_embedded_fonts = 1,
    },
    .change_flags = UPDATE_OSD,
};

#undef OPT_BASE_STRUCT
#define OPT_BASE_STRUCT struct mp_osd_render_opts

const struct m_sub_options mp_osd_render_sub_opts = {
    .opts = (const struct m_option[]){
        {"osd-bar-align-x", OPTF_FLOAT(osd_bar_align_x), .min = -1.0, .max = +1.0},
        {"osd-bar-align-y", OPTF_FLOAT(osd_bar_align_y), .min = -1.0, .max = +1.0},
        {"osd-bar-w", OPTF_FLOAT(osd_bar_w), .min = 1, .max = 100},
        {"osd-bar-h", OPTF_FLOAT(osd_bar_h), .min = 0.1, .max = 50},
        {"osd", OPTF_SUBSTRUCT(osd_style, osd_style_conf)},
        {"osd-scale", OPTF_FLOAT(osd_scale), .min = 0, .max = 100},
        {"osd-scale-by-window", OPTF_FLAG(osd_scale_by_window)},
        {"force-rgba-osd-rendering", OPTF_FLAG(force_rgba_osd)},
        {0}
    },
    .size = sizeof(OPT_BASE_STRUCT),
    .defaults = &(OPT_BASE_STRUCT){
        .osd_bar_align_y = 0.5,
        .osd_bar_w = 75.0,
        .osd_bar_h = 3.125,
        .osd_scale = 1,
        .osd_scale_by_window = 1,
    },
    .change_flags = UPDATE_OSD,
};

#undef OPT_BASE_STRUCT
#define OPT_BASE_STRUCT struct dvd_opts

const struct m_sub_options dvd_conf = {
    .opts = (const struct m_option[]){
        {"dvd-device", OPTF_STRING(device), .flags = M_OPT_FILE},
        {"dvd-speed", OPTF_INT(speed)},
        {"dvd-angle", OPTF_INT(angle), .min = 1, .max = 99},
        {0}
    },
    .size = sizeof(struct dvd_opts),
    .defaults = &(const struct dvd_opts){
        .angle = 1,
    },
};

#undef OPT_BASE_STRUCT
#define OPT_BASE_STRUCT struct filter_opts

const struct m_sub_options filter_conf = {
    .opts = (const struct m_option[]){
        {"deinterlace", OPTF_FLAG(deinterlace)},
        {0}
    },
    .size = sizeof(OPT_BASE_STRUCT),
    .change_flags = UPDATE_IMGPAR,
};

#undef OPT_BASE_STRUCT
#define OPT_BASE_STRUCT struct MPOpts

static const m_option_t mp_opts[] = {
    // handled in command line pre-parser (parse_commandline.c)
    {"v", &m_option_type_dummy_flag, CONF_NOCFG | M_OPT_NOPROP,
     .offset = -1},
    {"playlist", CONF_TYPE_STRING, CONF_NOCFG | M_OPT_FILE, .offset = -1},
    {"{", &m_option_type_dummy_flag, CONF_NOCFG | M_OPT_NOPROP,
     .offset = -1},
    {"}", &m_option_type_dummy_flag, CONF_NOCFG | M_OPT_NOPROP,
     .offset = -1},

    // handled in m_config.c
    { "include", CONF_TYPE_STRING, M_OPT_FILE, .offset = -1},
    { "profile", CONF_TYPE_STRING_LIST, 0, .offset = -1},
    { "show-profile", CONF_TYPE_STRING, CONF_NOCFG | M_OPT_NOPROP |
        M_OPT_OPTIONAL_PARAM,  .offset = -1},
    { "list-options", &m_option_type_dummy_flag, CONF_NOCFG | M_OPT_NOPROP,
        .offset = -1},
    {"list-properties", OPTF_FLAG(property_print_help), .flags = CONF_NOCFG | M_OPT_NOPROP},
    { "help", CONF_TYPE_STRING, CONF_NOCFG | M_OPT_NOPROP | M_OPT_OPTIONAL_PARAM,
        .offset = -1},
    { "h", CONF_TYPE_STRING, CONF_NOCFG | M_OPT_NOPROP | M_OPT_OPTIONAL_PARAM,
        .offset = -1},

    OPT_PRINT("list-protocols", stream_print_proto_list),
    OPT_PRINT("version", print_version),
    OPT_PRINT("V", print_version),

#if HAVE_TESTS
    {"unittest", OPTF_STRING(test_mode), .flags = CONF_NOCFG | M_OPT_NOPROP},
#endif

    OPT_CHOICE("player-operation-mode", operation_mode,
               M_OPT_PRE_PARSE | M_OPT_NOPROP,
               ({"cplayer", 0}, {"pseudo-gui", 1})),

    {"shuffle", OPTF_FLAG(shuffle)},

// ------------------------- common options --------------------
    {"quiet", OPTF_FLAG(quiet)},
    {"really-quiet", OPTF_FLAG(msg_really_quiet),
     .flags = CONF_PRE_PARSE | UPDATE_TERM},
    {"terminal", OPTF_FLAG(use_terminal), .flags = CONF_PRE_PARSE | UPDATE_TERM},
    OPT_GENERAL(char**, "msg-level", msg_levels, CONF_PRE_PARSE | UPDATE_TERM,
                .type = &m_option_type_msglevels),
    {"dump-stats", OPTF_STRING(dump_stats),
     .flags = UPDATE_TERM | CONF_PRE_PARSE | M_OPT_FILE},
    {"msg-color", OPTF_FLAG(msg_color), .flags = CONF_PRE_PARSE | UPDATE_TERM},
    {"log-file", OPTF_STRING(log_file),
     .flags = CONF_PRE_PARSE | M_OPT_FILE | UPDATE_TERM},
    {"msg-module", OPTF_FLAG(msg_module), .flags = UPDATE_TERM},
    {"msg-time", OPTF_FLAG(msg_time), .flags = UPDATE_TERM},
#if HAVE_WIN32_DESKTOP
    {"priority", OPTF_CHOICE(w32_priority, ,
               ({"no",          0},
                {"realtime",    REALTIME_PRIORITY_CLASS},
                {"high",        HIGH_PRIORITY_CLASS},
                {"abovenormal", ABOVE_NORMAL_PRIORITY_CLASS},
                {"normal",      NORMAL_PRIORITY_CLASS},
                {"belownormal", BELOW_NORMAL_PRIORITY_CLASS},
                {"idle",        IDLE_PRIORITY_CLASS})),
     .flags = UPDATE_PRIORITY},
#endif
    {"config", OPTF_FLAG(load_config), .flags = CONF_PRE_PARSE},
    {"config-dir", OPTF_STRING(force_configdir),
     .flags = CONF_NOCFG | CONF_PRE_PARSE | M_OPT_FILE},
    {"reset-on-next-file", OPTF_STRINGLIST(reset_options)},

#if HAVE_LUA || HAVE_JAVASCRIPT
    {"scripts", OPTF_PATHLIST(script_files), .flags = M_OPT_FILE},
    {"script", OPTF_CLI_ALIAS("scripts-append")},
    {"script-opts", OPTF_KEYVALUELIST(script_opts)},
    {"load-scripts", OPTF_FLAG(auto_load_scripts)},
#endif
#if HAVE_LUA
    {"osc", OPTF_FLAG(lua_load_osc), .flags = UPDATE_BUILTIN_SCRIPTS},
    {"ytdl", OPTF_FLAG(lua_load_ytdl), .flags = UPDATE_BUILTIN_SCRIPTS},
    {"ytdl-format", OPTF_STRING(lua_ytdl_format)},
    {"ytdl-raw-options", OPTF_KEYVALUELIST(lua_ytdl_raw_options)},
    {"load-stats-overlay", OPTF_FLAG(lua_load_stats),
     .flags = UPDATE_BUILTIN_SCRIPTS},
    {"load-osd-console", OPTF_FLAG(lua_load_console),
      .flags = UPDATE_BUILTIN_SCRIPTS},
#endif

// ------------------------- stream options --------------------

#if HAVE_DVDNAV
    {"", OPTF_SUBSTRUCT(dvd_opts, dvd_conf)},
#endif
    {"edition", OPTF_CHOICE(edition_id, ({"auto", -1})), .min = 0, .max = 8190},
#if HAVE_LIBBLURAY
    {"bluray-device", OPTF_STRING(bluray_device), .flags = M_OPT_FILE},
#endif /* HAVE_LIBBLURAY */

// ------------------------- demuxer options --------------------

    OPT_CHOICE_OR_INT("frames", play_frames, 0, 0, INT_MAX, ({"all", -1})),

    {"start", OPTF_REL_TIME(play_start)},
    {"end", OPTF_REL_TIME(play_end)},
    {"length", OPTF_REL_TIME(play_length)},

    {"play-dir", OPTF_CHOICE(play_dir,
     ({"forward", 1}, {"+", 1}, {"backward", -1}, {"-", -1}))},

    {"rebase-start-time", OPTF_FLAG(rebase_start_time)},

    {"ab-loop-a", OPTF_TIME(ab_loop[0]), .min = MP_NOPTS_VALUE},
    {"ab-loop-b", OPTF_TIME(ab_loop[1]), .min = MP_NOPTS_VALUE},
    {"ab-loop-count", OPTF_CHOICE(ab_loop_count, ({"inf", -1})),
     .min = 0, .max = INT_MAX},

    {"playlist-start", OPTF_CHOICE(playlist_pos, ({"auto", -1}, {"no", -1})),
     .min = 0, .max = INT_MAX},

    {"pause", OPTF_FLAG(pause)},
    OPT_CHOICE("keep-open", keep_open, 0,
               ({"no", 0},
                {"yes", 1},
                {"always", 2})),
    {"keep-open-pause", OPTF_FLAG(keep_open_pause)},
    {"image-display-duration", OPTF_DOUBLE(image_display_duration),
     .min = 0, .max = INFINITY},

     {"index", OPTF_CHOICE(index_mode, ({"default", 1}, {"recreate", 0}))},

    // select audio/video/subtitle stream
    OPT_TRACKCHOICE("aid", stream_id[0][STREAM_AUDIO]),
    OPT_TRACKCHOICE("vid", stream_id[0][STREAM_VIDEO]),
    OPT_TRACKCHOICE("sid", stream_id[0][STREAM_SUB]),
    OPT_TRACKCHOICE("secondary-sid", stream_id[1][STREAM_SUB]),
    {"sub", OPTF_ALIAS("sid")},
    {"video", OPTF_ALIAS("vid")},
    {"audio", OPTF_ALIAS("aid")},
    {"alang", OPTF_STRINGLIST(stream_lang[STREAM_AUDIO])},
    {"slang", OPTF_STRINGLIST(stream_lang[STREAM_SUB])},
    {"vlang", OPTF_STRINGLIST(stream_lang[STREAM_VIDEO])},
    {"track-auto-selection", OPTF_FLAG(stream_auto_sel)},

    {"lavfi-complex", OPTF_STRING(lavfi_complex), .flags = UPDATE_LAVFI_COMPLEX},

    {"audio-display", OPTF_CHOICE(audio_display, ({"no", 0}, {"attachment", 1}))},

    {"hls-bitrate", OPTF_CHOICE(hls_bitrate,
     ({"no", -1}, {"min", 0}, {"max", INT_MAX})), .min = 0, .max = INT_MAX},

    {"display-tags", OPTF_STRINGLIST(display_tags)},

#if HAVE_CDDA
    {"cdda", OPTF_SUBSTRUCT(stream_cdda_opts, stream_cdda_conf)},
    {"cdrom-device", OPTF_STRING(cdrom_device), .flags = M_OPT_FILE},
#endif

    // demuxer.c - select audio/sub file/demuxer
    {"audio-files", OPTF_PATHLIST(audio_files), .flags = M_OPT_FILE},
    {"audio-file", OPTF_CLI_ALIAS("audio-files-append")},
    {"demuxer", OPTF_STRING(demuxer_name)},
    {"audio-demuxer", OPTF_STRING(audio_demuxer_name)},
    {"sub-demuxer", OPTF_STRING(sub_demuxer_name)},
    {"demuxer-thread", OPTF_FLAG(demuxer_thread)},
    {"demuxer-termination-timeout", OPTF_DOUBLE(demux_termination_timeout)},
    {"demuxer-cache-wait", OPTF_FLAG(demuxer_cache_wait)},
    {"prefetch-playlist", OPTF_FLAG(prefetch_open)},
    {"cache-pause", OPTF_FLAG(cache_pause)},
    {"cache-pause-initial", OPTF_FLAG(cache_pause_initial)},
    {"cache-pause-wait", OPTF_FLOAT(cache_pause_wait), .min = 0, .max = DBL_MAX},

    {"mf-fps", OPTF_DOUBLE(mf_fps)},
    {"mf-type", OPTF_STRING(mf_type)},
#if HAVE_DVBIN
    {"dvbin", OPTF_SUBSTRUCT(stream_dvb_opts, stream_dvb_conf)},
#endif
    {"", OPTF_SUBSTRUCT(stream_lavf_opts, stream_lavf_conf)},

// ------------------------- a-v sync options --------------------

    // set A-V sync correction speed (0=disables it):
    {"mc", OPTF_FLOAT(default_max_pts_correction), .min = 0, .max = 100},

    {"audio-samplerate", OPTF_INT(force_srate), .flags = UPDATE_AUDIO,
      .min = 0, .max = 16*48000},
    {"audio-channels", OPTF_CHANNELS(audio_output_channels), .flags = UPDATE_AUDIO},
    {"audio-format", OPTF_AUDIOFORMAT(audio_output_format), .flags = UPDATE_AUDIO},
    {"speed", OPTF_DOUBLE(playback_speed), .min = 0.01, .max = 100.0},

    {"audio-pitch-correction", OPTF_FLAG(pitch_correction)},

    // set a-v distance
    {"audio-delay", OPTF_FLOAT(audio_delay)},

// ------------------------- codec/vfilter options --------------------

    OPT_SETTINGSLIST("af-defaults", af_defs, 0, &af_obj_list,
                     .deprecation_message = "use --af + enable/disable flags"),
    OPT_SETTINGSLIST("af", af_settings, 0, &af_obj_list, ),
    OPT_SETTINGSLIST("vf-defaults", vf_defs, 0, &vf_obj_list,
                     .deprecation_message = "use --vf + enable/disable flags"),
    OPT_SETTINGSLIST("vf", vf_settings, 0, &vf_obj_list, ),

    {"", OPTF_SUBSTRUCT(filter_opts, filter_conf)},

    {"", OPTF_SUBSTRUCT(dec_wrapper, dec_wrapper_conf)},
    {"", OPTF_SUBSTRUCT(vd_lavc_params, vd_lavc_conf)},
    {"ad-lavc", OPTF_SUBSTRUCT(ad_lavc_params, ad_lavc_conf)},

    {"", OPTF_SUBSTRUCT(demux_lavf, demux_lavf_conf)},
    {"demuxer-rawaudio", OPTF_SUBSTRUCT(demux_rawaudio, demux_rawaudio_conf)},
    {"demuxer-rawvideo", OPTF_SUBSTRUCT(demux_rawvideo, demux_rawvideo_conf)},
    {"demuxer-mkv", OPTF_SUBSTRUCT(demux_mkv, demux_mkv_conf)},
    {"demuxer-cue", OPTF_SUBSTRUCT(demux_cue, demux_cue_conf)},

// ------------------------- subtitles options --------------------

    {"sub-files", OPTF_PATHLIST(sub_name), .flags = M_OPT_FILE},
    {"sub-file", OPTF_CLI_ALIAS("sub-files-append")},
    {"sub-file-paths", OPTF_PATHLIST(sub_paths), .flags = M_OPT_FILE},
    {"audio-file-paths", OPTF_PATHLIST(audiofile_paths), .flags = M_OPT_FILE},
    {"external-files", OPTF_PATHLIST(external_files), .flags = M_OPT_FILE},
    {"external-file", OPTF_CLI_ALIAS("external-files-append")},
    {"autoload-files", OPTF_FLAG(autoload_files)},
    {"sub-auto", OPTF_CHOICE(sub_auto,
     ({"no", -1}, {"exact", 0}, {"fuzzy", 1}, {"all", 2}))},
    {"audio-file-auto", OPTF_CHOICE(audiofile_auto,
     ({"no", -1}, {"exact", 0}, {"fuzzy", 1}, {"all", 2}))},

    {"", OPTF_SUBSTRUCT(subs_rend, mp_subtitle_sub_opts)},
    {"", OPTF_SUBSTRUCT(subs_filt, mp_sub_filter_opts)},
    {"", OPTF_SUBSTRUCT(osd_rend, mp_osd_render_sub_opts)},

    {"osd-bar", OPTF_FLAG(osd_bar_visible), .flags = UPDATE_OSD},

//---------------------- libao/libvo options ------------------------
    {"", OPTF_SUBSTRUCT(ao_opts, ao_conf)},
    {"audio-exclusive", OPTF_FLAG(audio_exclusive), .flags = UPDATE_AUDIO},
    {"audio-fallback-to-null", OPTF_FLAG(ao_null_fallback)},
    {"audio-stream-silence", OPTF_FLAG(audio_stream_silence)},
    {"audio-wait-open", OPTF_FLOAT(audio_wait_open), .min = 0, .max = 60},
    {"force-window", OPTF_CHOICE(force_vo,
     ({"no", 0}, {"yes", 1}, {"immediate", 2}))},

    {"volume-max", OPTF_FLOAT(softvol_max), .min = 100, .max = 1000},
    // values <0 for volume and mute are legacy and ignored
    {"volume", OPTF_FLOAT(softvol_volume), .flags = UPDATE_VOL,
     .min = -1, .max = 1000},
    {"mute", OPTF_CHOICE(softvol_mute,
               ({"no", 0},
                {"auto", 0},
                {"yes", 1})),
     .flags = UPDATE_VOL},
    {"replaygain", OPTF_CHOICE(rgain_mode,
               ({"no", 0},
                {"track", 1},
                {"album", 2})),
     .flags = UPDATE_VOL},
    {"replaygain-preamp", OPTF_FLOAT(rgain_preamp), .flags = UPDATE_VOL,
     .min = -15, .max = 15},
    {"replaygain-clip", OPTF_FLAG(rgain_clip), .flags = UPDATE_VOL},
    {"replaygain-fallback", OPTF_FLOAT(rgain_fallback), .flags = UPDATE_VOL,
     .min = -200, .max = 60},
    {"gapless-audio", OPTF_CHOICE(gapless_audio,
               ({"no", 0},
                {"yes", 1},
                {"weak", -1}))},

    {"title", OPTF_STRING(wintitle)},
    {"force-media-title", OPTF_STRING(media_title)},

    {"cursor-autohide", OPTF_CHOICE(cursor_autohide_delay, ({"no", -1}, {"always", -2})), .min = 0, .max = 30000},
    {"cursor-autohide-fs-only", OPTF_FLAG(cursor_autohide_fs)},
    {"stop-screensaver", OPTF_FLAG(stop_screensaver), .flags = UPDATE_SCREENSAVER},

    {"", OPTF_SUBSTRUCT(video_equalizer, mp_csp_equalizer_conf)},

    {"use-filedir-conf", OPTF_FLAG(use_filedir_conf)},
    {"osd-level", OPTF_CHOICE(osd_level, ({"0", 0}, {"1", 1}, {"2", 2}, {"3", 3}))},
    {"osd-on-seek", OPTF_CHOICE(osd_on_seek,
               ({"no", 0},
                {"bar", 1},
                {"msg", 2},
                {"msg-bar", 3}))},
    {"osd-duration", OPTF_INT(osd_duration), .min = 0, .max = 3600000},
    {"osd-fractions", OPTF_FLAG(osd_fractions)},

    {"sstep", OPTF_DOUBLE(step_sec), .min = 0, .max = DBL_MAX},

    {"framedrop", OPTF_CHOICE(frame_dropping,
               ({"no", 0},
                {"vo", 1},
                {"decoder", 2},
                {"decoder+vo", 3}))},
    {"video-latency-hacks", OPTF_FLAG(video_latency_hacks)},

    {"untimed", OPTF_FLAG(untimed)},

    {"stream-dump", OPTF_STRING(stream_dump), .flags = M_OPT_FILE},

    {"stop-playback-on-init-failure", OPTF_FLAG(stop_playback_on_init_failure)},

    {"loop-playlist", OPTF_CHOICE(loop_times,
                      ({"no", 1},
                       {"inf", -1}, {"yes", -1},
                       {"force", -2})),
     .min = 1, .max = 10000},
    {"loop-file", OPTF_CHOICE(loop_file,
                      ({"no", 0},
                       {"yes", -1},
                       {"inf", -1})),
     .min = 0, .max = 10000},
    {"loop", OPTF_ALIAS("loop-file")},

    {"resume-playback", OPTF_FLAG(position_resume)},
    {"resume-playback-check-mtime", OPTF_FLAG(position_check_mtime)},
    {"save-position-on-quit", OPTF_FLAG(position_save_on_quit)},
    {"write-filename-in-watch-later-config",
     OPTF_FLAG(write_filename_in_watch_later_config)},
    {"ignore-path-in-watch-later-config",
     OPTF_FLAG(ignore_path_in_watch_later_config)},
    {"watch-later-directory", OPTF_STRING(watch_later_directory),
     .flags = M_OPT_FILE},

    {"ordered-chapters", OPTF_FLAG(ordered_chapters)},
    {"ordered-chapters-files", OPTF_STRING(ordered_chapters_files),
     .flags = M_OPT_FILE},
    {"chapter-merge-threshold", OPTF_INT(chapter_merge_threshold),
     .min = 0, .max = 10000},

    {"chapter-seek-threshold", OPTF_DOUBLE(chapter_seek_threshold)},

    {"chapters-file", OPTF_STRING(chapter_file), .flags = M_OPT_FILE},

    {"merge-files", OPTF_FLAG(merge_files)},

    // a-v sync stuff:
    {"initial-audio-sync", OPTF_FLAG(initial_audio_sync)},
    {"video-sync", OPTF_CHOICE(video_sync,
               ({"audio", VS_DEFAULT},
                {"display-resample", VS_DISP_RESAMPLE},
                {"display-resample-vdrop", VS_DISP_RESAMPLE_VDROP},
                {"display-resample-desync", VS_DISP_RESAMPLE_NONE},
                {"display-adrop", VS_DISP_ADROP},
                {"display-vdrop", VS_DISP_VDROP},
                {"display-desync", VS_DISP_NONE},
                {"desync", VS_NONE}))},
    {"video-sync-max-video-change", OPTF_DOUBLE(sync_max_video_change),
     .min = 0, .max = DBL_MAX},
    {"video-sync-max-audio-change", OPTF_DOUBLE(sync_max_audio_change),
     .min = 0, .max = 1},
    {"video-sync-adrop-size", OPTF_DOUBLE(sync_audio_drop_size),
     .min = 0, .max = 1},
    {"hr-seek", OPTF_CHOICE(hr_seek,
     ({"no", -1}, {"absolute", 0}, {"yes", 1}, {"always", 1}, {"default", 2}))},
    {"hr-seek-demuxer-offset", OPTF_FLOAT(hr_seek_demuxer_offset)},
    {"hr-seek-framedrop", OPTF_FLAG(hr_seek_framedrop)},
    {"autosync", OPTF_CHOICE(autosync, ({"no", -1})), .min = 0, .max = 10000},

    {"term-osd", OPTF_CHOICE(term_osd,
     ({"force", 1}, {"auto", 2}, {"no", 0}))},

    {"term-osd-bar", OPTF_FLAG(term_osd_bar)},
    {"term-osd-bar-chars", OPTF_STRING(term_osd_bar_chars)},

    {"term-playing-msg", OPTF_STRING(playing_msg)},
    {"osd-playing-msg", OPTF_STRING(osd_playing_msg)},
    {"term-status-msg", OPTF_STRING(status_msg)},
    {"osd-status-msg", OPTF_STRING(osd_status_msg)},
    {"osd-msg1", OPTF_STRING(osd_msg[0])},
    {"osd-msg2", OPTF_STRING(osd_msg[1])},
    {"osd-msg3", OPTF_STRING(osd_msg[2])},

    {"video-osd", OPTF_FLAG(video_osd)},

    {"idle", OPTF_CHOICE(player_idle_mode,
     ({"no",   0}, {"once", 1}, {"yes",  2}))},

    {"input-terminal", OPTF_FLAG(consolecontrols), .flags = UPDATE_TERM},

    {"input-file", OPTF_STRING(input_file),
     .flags = M_OPT_FILE, .deprecation_message = "use --input-ipc-server"},
    {"input-ipc-server", OPTF_STRING(ipc_path), .flags = M_OPT_FILE},

    {"screenshot", OPTF_SUBSTRUCT(screenshot_image_opts, screenshot_conf)},
    {"screenshot-template", OPTF_STRING(screenshot_template)},
    {"screenshot-directory", OPTF_STRING(screenshot_directory),
     .flags = M_OPT_FILE},

    {"record-file", OPTF_STRING(record_file), .flags = M_OPT_FILE,
     .deprecation_message = "use --stream-record or the dump-cache command"},

    {"", OPTF_SUBSTRUCT(resample_opts, resample_conf)},

    {"", OPTF_SUBSTRUCT(input_opts, input_config)},

    {"", OPTF_SUBSTRUCT(vo, vo_sub_opts)},
    {"", OPTF_SUBSTRUCT(demux_opts, demux_conf)},
    {"", OPTF_SUBSTRUCT(demux_cache_opts, demux_cache_conf)},
    {"", OPTF_SUBSTRUCT(stream_opts, stream_conf)},

    {"", OPTF_SUBSTRUCT(gl_video_opts, gl_video_conf)},
    {"", OPTF_SUBSTRUCT(spirv_opts, spirv_conf)},

#if HAVE_GL
    {"", OPTF_SUBSTRUCT(opengl_opts, opengl_conf)},
#endif

#if HAVE_VULKAN
    {"", OPTF_SUBSTRUCT(vulkan_opts, vulkan_conf)},
#endif

#if HAVE_D3D11
    {"", OPTF_SUBSTRUCT(d3d11_opts, d3d11_conf)},
#if HAVE_D3D_HWACCEL
    {"", OPTF_SUBSTRUCT(d3d11va_opts, d3d11va_conf)},
#endif
#endif

#if HAVE_EGL_ANGLE_WIN32
    {"", OPTF_SUBSTRUCT(angle_opts, angle_conf)},
#endif

#if HAVE_GL_COCOA
    {"", OPTF_SUBSTRUCT(cocoa_opts, cocoa_conf)},
#endif

#if HAVE_COCOA
    {"", OPTF_SUBSTRUCT(macos_opts, macos_conf)},
#endif

#if HAVE_EGL_ANDROID
    {"", OPTF_SUBSTRUCT(android_opts, android_conf)},
#endif

#if HAVE_WAYLAND
    {"", OPTF_SUBSTRUCT(wayland_opts, wayland_conf)},
#endif

#if HAVE_GL_WIN32
    {"opengl-dwmflush", OPTF_CHOICE(wingl_dwm_flush,
     ({"no", -1}, {"auto", 0}, {"windowed", 1}, {"yes", 2}))},
#endif

#if HAVE_CUDA_HWACCEL
    {"cuda-decode-device", OPTF_CHOICE(cuda_device, ({"auto", -1})), .min = 0, .max = INT_MAX},
#endif

#if HAVE_VAAPI
    {"vaapi", OPTF_SUBSTRUCT(vaapi_opts, vaapi_conf)},
#endif

    {"sws", OPTF_SUBSTRUCT(sws_opts, sws_conf)},

#if HAVE_ZIMG
    {"zimg", OPTF_SUBSTRUCT(zimg_opts, zimg_conf)},
#endif

    {"", OPTF_SUBSTRUCT(encode_opts, encode_config)},

    {"a52drc", OPTF_REMOVED("use --ad-lavc-ac3drc=level")},
    {"afm", OPTF_REMOVED("use --ad=...")},
    {"aspect", OPTF_REPLACED("video-aspect-override")},
    {"ass-bottom-margin", OPTF_REMOVED("use --vf=sub=bottom:top")},
    {"ass", OPTF_REPLACED("sub-ass")},
    {"audiofile", OPTF_REPLACED("audio-file")},
    {"benchmark", OPTF_REMOVED("use --untimed (no stats)")},
    {"capture", OPTF_REMOVED(NULL)},
    {"stream-capture", OPTF_REMOVED(NULL)},
    {"channels", OPTF_REMOVED("use --audio-channels (changed semantics)")},
    {"cursor-autohide-delay", OPTF_REPLACED("cursor-autohide")},
    {"delay", OPTF_REPLACED("audio-delay")},
    {"dumpstream", OPTF_REMOVED("use --stream-dump=<filename>")},
    {"dvdangle", OPTF_REPLACED("dvd-angle")},
    {"endpos", OPTF_REPLACED("length")},
    {"font", OPTF_REPLACED("osd-font")},
    {"forcedsubsonly", OPTF_REPLACED("sub-forced-only")},
    {"format", OPTF_REPLACED("audio-format")},
    {"hardframedrop", OPTF_REMOVED(NULL)},
    {"identify", OPTF_REMOVED("use TOOLS/mpv_identify.sh")},
    {"lavdopts", OPTF_REMOVED("use --vd-lavc-...")},
    {"lavfdopts", OPTF_REMOVED("use --demuxer-lavf-...")},
    {"lua", OPTF_REPLACED("script")},
    {"lua-opts", OPTF_REPLACED("script-opts")},
    {"mixer-channel", OPTF_REMOVED("use AO suboptions (alsa, oss)")},
    {"mixer", OPTF_REMOVED("use AO suboptions (alsa, oss)")},
    {"mouse-movements", OPTF_REPLACED("input-cursor")},
    {"msgcolor", OPTF_REPLACED("msg-color")},
    {"msglevel", OPTF_REMOVED("use --msg-level (changed semantics)")},
    {"msgmodule", OPTF_REPLACED("msg-module")},
    {"name", OPTF_REPLACED("x11-name")},
    {"noar", OPTF_REPLACED("no-input-appleremote")},
    {"noautosub", OPTF_REPLACED("no-sub-auto")},
    {"noconsolecontrols", OPTF_REPLACED("no-input-terminal")},
    {"nosound", OPTF_REPLACED("no-audio")},
    {"osdlevel", OPTF_REPLACED("osd-level")},
    {"panscanrange", OPTF_REMOVED("use --video-zoom, --video-pan-x/y")},
    {"playing-msg", OPTF_REPLACED("term-playing-msg")},
    {"pp", OPTF_REMOVED(NULL)},
    {"pphelp", OPTF_REMOVED(NULL)},
    {"rawaudio", OPTF_REMOVED("use --demuxer-rawaudio-...")},
    {"rawvideo", OPTF_REMOVED("use --demuxer-rawvideo-...")},
    {"spugauss", OPTF_REPLACED("sub-gauss")},
    {"srate", OPTF_REPLACED("audio-samplerate")},
    {"ss", OPTF_REPLACED("start")},
    {"stop-xscreensaver", OPTF_REPLACED("stop-screensaver")},
    {"sub-fuzziness", OPTF_REPLACED("sub-auto")},
    {"subcp", OPTF_REPLACED("sub-codepage")},
    {"subdelay", OPTF_REPLACED("sub-delay")},
    {"subfile", OPTF_REPLACED("sub-file")},
    {"subfont-text-scale", OPTF_REPLACED("sub-scale")},
    {"subfont", OPTF_REPLACED("sub-text-font")},
    {"subfps", OPTF_REPLACED("sub-fps")},
    {"subpos", OPTF_REPLACED("sub-pos")},
    {"tvscan", OPTF_REPLACED("tv-scan")},
    {"use-filename-title", OPTF_REMOVED("use --title='${filename}'")},
    {"vc", OPTF_REMOVED("use --vd=..., --hwdec=...")},
    {"vobsub", OPTF_REMOVED("use --sub-file (pass the .idx file)")},
    {"xineramascreen", OPTF_REMOVED("use --screen (different values)")},
    {"xy", OPTF_REMOVED("use --autofit")},
    {"zoom", OPTF_REMOVED("Inverse available as ``--video-unscaled")},
    {"media-keys", OPTF_REPLACED("input-media-keys")},
    {"right-alt-gr", OPTF_REPLACED("input-right-alt-gr")},
    {"autosub", OPTF_REPLACED("sub-auto")},
    {"autosub-match", OPTF_REPLACED("sub-auto")},
    {"status-msg", OPTF_REPLACED("term-status-msg")},
    {"idx", OPTF_REPLACED("index")},
    {"forceidx", OPTF_REPLACED("index")},
    {"cache-pause-below", OPTF_REMOVED("for 'no', use --no-cache-pause")},
    {"no-cache-pause-below", OPTF_REMOVED("use --no-cache-pause")},
    {"volstep", OPTF_REMOVED("edit input.conf directly instead")},
    {"fixed-vo", OPTF_REMOVED("--fixed-vo=yes is now the default")},
    {"mkv-subtitle-preroll", OPTF_REPLACED("demuxer-mkv-subtitle-preroll")},
    {"ass-use-margins", OPTF_REPLACED("sub-use-margins")},
    {"media-title", OPTF_REPLACED("force-media-title")},
    {"input-unix-socket", OPTF_REPLACED("input-ipc-server")},
    {"softvol-max", OPTF_REPLACED("volume-max")},
    {"bluray-angle", OPTF_REMOVED("this didn't do anything for a few releases")},
    {"sub-text-font", OPTF_REPLACED("sub-font")},
    {"sub-text-font-size", OPTF_REPLACED("sub-font-size")},
    {"sub-text-color", OPTF_REPLACED("sub-color")},
    {"sub-text-border-color", OPTF_REPLACED("sub-border-color")},
    {"sub-text-shadow-color", OPTF_REPLACED("sub-shadow-color")},
    {"sub-text-back-color", OPTF_REPLACED("sub-back-color")},
    {"sub-text-border-size", OPTF_REPLACED("sub-border-size")},
    {"sub-text-shadow-offset", OPTF_REPLACED("sub-shadow-offset")},
    {"sub-text-spacing", OPTF_REPLACED("sub-spacing")},
    {"sub-text-margin-x", OPTF_REPLACED("sub-margin-x")},
    {"sub-text-margin-y", OPTF_REPLACED("sub-margin-y")},
    {"sub-text-align-x", OPTF_REPLACED("sub-align-x")},
    {"sub-text-align-y", OPTF_REPLACED("sub-align-y")},
    {"sub-text-blur", OPTF_REPLACED("sub-blur")},
    {"sub-text-bold", OPTF_REPLACED("sub-bold")},
    {"sub-text-italic", OPTF_REPLACED("sub-italic")},
    {"ass-line-spacing", OPTF_REPLACED("sub-ass-line-spacing")},
    {"ass-force-margins", OPTF_REPLACED("sub-ass-force-margins")},
    {"ass-vsfilter-aspect-compat", OPTF_REPLACED("sub-ass-vsfilter-aspect-compat")},
    {"ass-vsfilter-color-compat", OPTF_REPLACED("sub-ass-vsfilter-color-compat")},
    {"ass-vsfilter-blur-compat", OPTF_REPLACED("sub-ass-vsfilter-blur-compat")},
    {"ass-force-style", OPTF_REPLACED("sub-ass-force-style")},
    {"ass-styles", OPTF_REPLACED("sub-ass-styles")},
    {"ass-hinting", OPTF_REPLACED("sub-ass-hinting")},
    {"ass-shaper", OPTF_REPLACED("sub-ass-shaper")},
    {"ass-style-override", OPTF_REPLACED("sub-ass-style-override")},
    {"ass-scale-with-window", OPTF_REPLACED("sub-ass-scale-with-window")},
    {"sub-ass-style-override", OPTF_REPLACED("sub-ass-override")},
    {"fs-black-out-screens", OPTF_REMOVED(NULL)},
    {"sub-paths", OPTF_REPLACED("sub-file-paths")},
    {"heartbeat-cmd", OPTF_REMOVED("use Lua scripting instead")},
    {"no-ometadata", OPTF_REMOVED("use --no-ocopy-metadata")},
    {"video-stereo-mode", OPTF_REMOVED("removed, try --vf=stereo3d")},
    {"chapter", OPTF_REMOVED("use '--start=#123' '--end=#124' (for chapter 123)")},
    {"video-aspect", OPTF_REPLACED("video-aspect-override")},
    {"display-fps", OPTF_REPLACED("override-display-fps")},

    {0}
};

static const struct MPOpts mp_default_opts = {
    .use_terminal = 1,
    .msg_color = 1,
    .softvol_max = 130,
    .softvol_volume = 100,
    .softvol_mute = 0,
    .gapless_audio = -1,
    .wintitle = "${?media-title:${media-title}}${!media-title:No file} - mpv",
    .stop_screensaver = 1,
    .cursor_autohide_delay = 1000,
    .video_osd = 1,
    .osd_level = 1,
    .osd_on_seek = 1,
    .osd_duration = 1000,
#if HAVE_LUA
    .lua_load_osc = 1,
    .lua_load_ytdl = 1,
    .lua_ytdl_format = NULL,
    .lua_ytdl_raw_options = NULL,
    .lua_load_stats = 1,
    .lua_load_console = 1,
#endif
    .auto_load_scripts = 1,
    .loop_times = 1,
    .ordered_chapters = 1,
    .chapter_merge_threshold = 100,
    .chapter_seek_threshold = 5.0,
    .hr_seek = 2,
    .hr_seek_framedrop = 1,
    .sync_max_video_change = 1,
    .sync_max_audio_change = 0.125,
    .sync_audio_drop_size = 0.020,
    .load_config = 1,
    .position_resume = 1,
    .autoload_files = 1,
    .demuxer_thread = 1,
    .demux_termination_timeout = 0.1,
    .hls_bitrate = INT_MAX,
    .cache_pause = 1,
    .cache_pause_wait = 1.0,
    .ab_loop = {MP_NOPTS_VALUE, MP_NOPTS_VALUE},
    .ab_loop_count = -1,
    .edition_id = -1,
    .default_max_pts_correction = -1,
    .initial_audio_sync = 1,
    .frame_dropping = 1,
    .term_osd = 2,
    .term_osd_bar_chars = "[-+-]",
    .consolecontrols = 1,
    .playlist_pos = -1,
    .play_frames = -1,
    .rebase_start_time = 1,
    .keep_open = 0,
    .keep_open_pause = 1,
    .image_display_duration = 1.0,
    .stream_id = { { [STREAM_AUDIO] = -1,
                     [STREAM_VIDEO] = -1,
                     [STREAM_SUB] = -1, },
                   { [STREAM_AUDIO] = -2,
                     [STREAM_VIDEO] = -2,
                     [STREAM_SUB] = -2, }, },
    .stream_auto_sel = 1,
    .audio_display = 1,
    .audio_output_format = 0,  // AF_FORMAT_UNKNOWN
    .playback_speed = 1.,
    .pitch_correction = 1,
    .sub_auto = 0,
    .audiofile_auto = -1,
    .osd_bar_visible = 1,
    .screenshot_template = "mpv-shot%n",
    .play_dir = 1,

    .audio_output_channels = {
        .set = 1,
        .auto_safe = 1,
    },

    .index_mode = 1,

    .mf_fps = 1.0,

    .display_tags = (char **)(const char*[]){
        "Artist", "Album", "Album_Artist", "Comment", "Composer",
        "Date", "Description", "Genre", "Performer", "Rating",
        "Series", "Title", "Track", "icy-title", "service_name",
        NULL
    },

    .cuda_device = -1,
};

const struct m_sub_options mp_opt_root = {
    .opts = mp_opts,
    .size = sizeof(struct MPOpts),
    .defaults = &mp_default_opts,
};

#endif /* MPLAYER_CFG_MPLAYER_H */
