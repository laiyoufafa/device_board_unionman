/*
*
* SPDX-License-Identifier: GPL-2.0
*
* Copyright (C) 2011-2018 ARM or its affiliates
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; version 2.
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/

#include <linux/version.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>

#include "acamera_logger.h"

#include "isp-v4l2-common.h"
#include "isp-v4l2-ctrl.h"
#include "fw-interface.h"

static int isp_v4l2_ctrl_check_valid( struct v4l2_ctrl *ctrl )
{
    if ( ctrl->is_int == 1 ) {
        if ( ctrl->val < ctrl->minimum || ctrl->val > ctrl->maximum )
            return -EINVAL;
    }

    return 0;
}

static int isp_v4l2_ctrl_s_ctrl_standard( struct v4l2_ctrl *ctrl )
{
    int ret = 0;
	struct v4l2_ctrl_handler *hdl = ctrl->handler;

    isp_v4l2_ctrl_t *isp_ctrl = std_hdl_to_isp_ctrl( hdl );
    int ctx_id = isp_ctrl->ctx_id;

    LOG( LOG_DEBUG, "Control - id:0x%x, val:%d, is_int:%d, min:%d, max:%d.\n",
         ctrl->id, ctrl->val, ctrl->is_int, ctrl->minimum, ctrl->maximum );

    if ( isp_v4l2_ctrl_check_valid( ctrl ) < 0 ) {
        return -EINVAL;
    }

    switch ( ctrl->id ) {
    case V4L2_CID_BRIGHTNESS:
        ret = fw_intf_set_brightness( ctx_id, ctrl->val );
        break;
    case V4L2_CID_CONTRAST:
        ret = fw_intf_set_contrast( ctx_id, ctrl->val );
        break;
    case V4L2_CID_SATURATION:
        ret = fw_intf_set_saturation( ctx_id, ctrl->val );
        break;
    case V4L2_CID_HUE:
        ret = fw_intf_set_hue( ctx_id, ctrl->val );
        break;
    case V4L2_CID_SHARPNESS:
        ret = fw_intf_set_sharpness( ctx_id, ctrl->val );
        break;
    case V4L2_CID_COLORFX:
        ret = fw_intf_set_color_fx( ctx_id, ctrl->val );
        break;
    case V4L2_CID_HFLIP:
        ret = fw_intf_set_hflip( ctx_id, ctrl->val );
        break;
    case V4L2_CID_VFLIP:
        ret = fw_intf_set_vflip( ctx_id, ctrl->val );
        break;
    case V4L2_CID_AUTOGAIN:
        ret = fw_intf_set_autogain( ctx_id, ctrl->val );
        break;
    case V4L2_CID_GAIN:
        ret = fw_intf_set_gain( ctx_id, ctrl->val );
        break;
    case V4L2_CID_EXPOSURE_AUTO:
        ret = fw_intf_set_exposure_auto( ctx_id, ctrl->val );
        break;
    case V4L2_CID_EXPOSURE_ABSOLUTE:
        ret = fw_intf_set_exposure( ctx_id, ctrl->val );
        break;
    case V4L2_CID_EXPOSURE_AUTO_PRIORITY:
        ret = fw_intf_set_variable_frame_rate( ctx_id, ctrl->val );
        break;
    case V4L2_CID_AUTO_WHITE_BALANCE:
        ret = fw_intf_set_white_balance_auto( ctx_id, ctrl->val );
        break;
    case V4L2_CID_WHITE_BALANCE_TEMPERATURE:
        ret = fw_intf_set_white_balance( ctx_id, ctrl->val );
        break;
    case V4L2_CID_FOCUS_AUTO:
        ret = fw_intf_set_focus_auto( ctx_id, ctrl->val );
        break;
    case V4L2_CID_FOCUS_ABSOLUTE:
        ret = fw_intf_set_focus( ctx_id, ctrl->val );
        break;
    }

    return ret;
}

static int isp_v4l2_ctrl_g_ctrl_standard( struct v4l2_ctrl *ctrl )
{
    int ret = 0;
	struct v4l2_ctrl_handler *hdl = ctrl->handler;

    isp_v4l2_ctrl_t *isp_ctrl = std_hdl_to_isp_ctrl( hdl );
    int ctx_id = isp_ctrl->ctx_id;

    LOG( LOG_DEBUG, "Control - id:0x%x.\n", ctrl->id);

    switch ( ctrl->id ) {
        case V4L2_CID_EXPOSURE_AUTO:
            ctrl->val = fw_intf_get_exposure_auto( ctx_id );
            break;
        case V4L2_CID_EXPOSURE_ABSOLUTE:
            ctrl->val = fw_intf_get_exposure( ctx_id );
            break;
        case V4L2_CID_AUTO_WHITE_BALANCE:
            ctrl->val = fw_intf_get_white_balance_auto( ctx_id );
            break;
    }

    return ret;
}

static int isp_v4l2_ctrl_s_ctrl_custom( struct v4l2_ctrl *ctrl )
{
    int ret = 0;
    struct v4l2_ctrl_handler *hdl = ctrl->handler;

    isp_v4l2_ctrl_t *isp_ctrl = cst_hdl_to_isp_ctrl( hdl );
    int ctx_id = isp_ctrl->ctx_id;

    LOG( LOG_DEBUG, "Control - id:0x%x, val:%d, is_int:%d, min:%d, max:%d.\n",
         ctrl->id, ctrl->val, ctrl->is_int, ctrl->minimum, ctrl->maximum);

    if ( isp_v4l2_ctrl_check_valid( ctrl ) < 0 ) {
        LOG( LOG_ERR, "Invalid param: id:0x%x, val:0x%x, is_int:%d, min:0x%x, max:0x%x.\n",
             ctrl->id, ctrl->val, ctrl->is_int, ctrl->minimum, ctrl->maximum );

        return -EINVAL;
    }

    switch ( ctrl->id ) {
    case ISP_V4L2_CID_TEST_PATTERN:
        LOG( LOG_INFO, "new test_pattern: %d.\n", ctrl->val );
        ret = fw_intf_set_test_pattern( ctx_id, ctrl->val );
        break;
    case ISP_V4L2_CID_TEST_PATTERN_TYPE:
        LOG( LOG_INFO, "new test_pattern_type: %d.\n", ctrl->val );
        ret = fw_intf_set_test_pattern_type( ctx_id, ctrl->val );
        break;
    case ISP_V4L2_CID_AF_REFOCUS:
        LOG( LOG_INFO, "new focus: %d.\n", ctrl->val );
        ret = fw_intf_set_af_refocus( ctx_id, ctrl->val );
        break;
    case ISP_V4L2_CID_SENSOR_PRESET:
        LOG( LOG_INFO, "new sensor preset: %d.\n", ctrl->val );
        ret = fw_intf_isp_set_sensor_preset( ctx_id, ctrl->val );
        break;
    case ISP_V4L2_CID_AF_ROI:
        // map [0,127] to [0, 254] due to limitaton of V4L2_CTRL_TYPE_INTEGER.
        LOG( LOG_INFO, "new af roi: 0x%x.\n", ctrl->val * 2 );
        ret = fw_intf_set_af_roi( ctx_id, ctrl->val * 2 );
        break;
    case ISP_V4L2_CID_OUTPUT_FR_ON_OFF:
        LOG( LOG_INFO, "output FR on/off: 0x%x.\n", ctrl->val );
        ret = fw_intf_set_output_fr_on_off( ctx_id, ctrl->val );
        break;
    case ISP_V4L2_CID_OUTPUT_DS1_ON_OFF:
        LOG( LOG_INFO, "output DS1 on/off: 0x%x.\n", ctrl->val );
        ret = fw_intf_set_output_ds1_on_off( ctx_id, ctrl->val );
        break;
    case ISP_V4L2_CID_CUSTOM_SENSOR_WDR_MODE:
        LOG( LOG_INFO, "set custom wdr mode: 0x%x.\n", ctrl->val );
        ret = fw_intf_set_custom_sensor_wdr_mode(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SENSOR_EXPOSURE:
        LOG( LOG_INFO, "set custom exposure: 0x%x.\n", ctrl->val );
        ret = fw_intf_set_custom_sensor_exposure(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SET_FR_FPS:
        LOG( LOG_INFO, "set fr fps: 0x%x.\n", ctrl->val );
        ret = fw_intf_set_custom_fr_fps(ctx_id, ctrl->val);
        *(ctrl->p_new.p_s32) = 0;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_SENSOR_TESTPATTERN:
        LOG( LOG_INFO, "set sensor test pattern: 0x%x.\n", ctrl->val );
        ret = fw_intf_set_custom_sensor_testpattern(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SENSOR_IR_CUT:
        LOG( LOG_INFO, "set_customer_sensor_ir_cut = %d\n", ctrl->val );
        ret = fw_intf_set_customer_sensor_ir_cut(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SET_AE_ZONE_WEIGHT:
        LOG( LOG_INFO, "set ae zone weight: 0x%llx.\n", *(ctrl->p_new.p_s64));
        ret = fw_intf_set_customer_ae_zone_weight(ctx_id, *(ctrl->p_new.p_s64));
        *(ctrl->p_new.p_s64) = 0;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_AWB_ZONE_WEIGHT:
        LOG( LOG_INFO, "set awb zone weight: 0x%llx.\n", *(ctrl->p_new.p_s64));
        ret = fw_intf_set_customer_awb_zone_weight(ctx_id, *(ctrl->p_new.p_s64));
        *(ctrl->p_new.p_s64) = 0;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_MANUAL_EXPOSURE:
        LOG( LOG_INFO, "set_customer_manual_exposure = %d\n", ctrl->val );
        ret = fw_intf_set_customer_manual_exposure(ctx_id, ctrl->val);
        ctrl->val = -1;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_SENSOR_INTEGRATION_TIME:
        LOG( LOG_INFO, "set_customer_integration_time = %d\n", ctrl->val );
        ret = fw_intf_set_customer_sensor_integration_time(ctx_id, ctrl->val);
        ctrl->val = -1;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_SENSOR_ANALOG_GAIN:
        LOG( LOG_INFO, "set_customer_sensor_analog_gain = %d\n", ctrl->val );
        ret = fw_intf_set_customer_sensor_analog_gain(ctx_id, ctrl->val);
        ctrl->val = -1;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_ISP_DIGITAL_GAIN:
        LOG( LOG_INFO, "set_customer_isp_digital_gain = %d\n", ctrl->val );
        ret = fw_intf_set_customer_isp_digital_gain(ctx_id, ctrl->val);
        ctrl->val = -1;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_STOP_SENSOR_UPDATE:
        LOG( LOG_INFO, "set_customer_stop_sensor_update = %d\n", ctrl->val );
        ret = fw_intf_set_customer_stop_sensor_update(ctx_id, ctrl->val);
        ctrl->val = -1;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_DS1_FPS:
        LOG( LOG_INFO, "set ds1 fps: 0x%x.\n", ctrl->val );
        ret = fw_intf_set_custom_ds1_fps(ctx_id, ctrl->val);
        *(ctrl->p_new.p_s32) = 0;
        break;
    case ISP_V4L2_CID_AE_COMPENSATION:
        LOG( LOG_INFO, "new ae compensation: %d.\n", ctrl->val );
        ret = fw_intf_set_ae_compensation( ctx_id, ctrl->val );
        break;
    case ISP_V4L2_CID_CUSTOM_SET_SENSOR_DIGITAL_GAIN:
        LOG( LOG_INFO, "set_customer_sensor_digital_gain = %d\n", ctrl->val );
        ret = fw_intf_set_customer_sensor_digital_gain(ctx_id, ctrl->val);
        ctrl->val = -1;
        break;
    case ISP_V4L2_CID_CUSTOM_SET_AWB_RED_GAIN:
        LOG( LOG_INFO, "set_customer_awb_red_gain = %d\n", ctrl->val );
        ret = fw_intf_set_customer_awb_red_gain(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SET_AWB_BLUE_GAIN:
        LOG( LOG_INFO, "set_customer_awb_blue_gain = %d\n", ctrl->val );
        ret = fw_intf_set_customer_awb_blue_gain(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SET_MAX_INTEGRATION_TIME:
        LOG( LOG_INFO, "set_customer_max_integration_time = %d\n", ctrl->val );
        ret = fw_intf_set_customer_max_integration_time(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SENSOR_FPS:
        LOG( LOG_INFO, "set custom fps: %d.\n", ctrl->val );
        ret = fw_intf_set_custom_sensor_fps(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SNR_MANUAL:
        LOG( LOG_INFO, "set snr manual: %d.\n", ctrl->val );
        ret = fw_intf_set_custom_snr_manual(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_SNR_STRENGTH:
        LOG( LOG_INFO, "set snr strength: %d.\n", ctrl->val );
        ret = fw_intf_set_custom_snr_strength(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_TNR_MANUAL:
        LOG( LOG_INFO, "set tnr manual: %d.\n", ctrl->val );
        ret = fw_intf_set_custom_tnr_manual(ctx_id, ctrl->val);
        break;
    case ISP_V4L2_CID_CUSTOM_TNR_OFFSET:
        LOG( LOG_INFO, "set tnr offset: %d.\n", ctrl->val );
        ret = fw_intf_set_custom_tnr_offset(ctx_id, ctrl->val);
        break;
     case ISP_V4L2_CID_CUSTOM_TEMPER_MODE:
        LOG( LOG_INFO, "set temper mode: %d.\n", ctrl->val );
        ret = fw_intf_set_customer_temper_mode(ctx_id, ctrl->val);
        break;
     case ISP_V4L2_CID_CUSTOM_WDR_SWITCH:
        LOG( LOG_INFO, "set wdr mode: %d.\n", ctrl->val );
        ret = fw_intf_set_customer_sensor_mode(ctx_id, ctrl->val);
        break;
     case ISP_V4L2_CID_CUSTOM_ANTIFLICKER:
        LOG( LOG_INFO, "set anti flicker: %d.\n", ctrl->val );
        ret = fw_intf_set_customer_antiflicker(ctx_id, ctrl->val);
        break;
     case ISP_V4L2_CID_CUSTOM_DEFOG_SWITCH:
        LOG( LOG_INFO, "set defog mode: %d.\n", ctrl->val );
        ret = fw_intf_set_customer_defog_mode(ctx_id, ctrl->val);
        break;
     case ISP_V4L2_CID_CUSTOM_DEFOG_STRENGTH:
        LOG( LOG_INFO, "set defog strength: %d.\n", ctrl->val );
        ret = fw_intf_set_customer_defog_ratio(ctx_id, ctrl->val);
        break;
    }

    return ret;
}

static int isp_v4l2_ctrl_g_ctrl_custom( struct v4l2_ctrl *ctrl )
{
    int ret = 0;
    struct v4l2_ctrl_handler *hdl = ctrl->handler;

    isp_v4l2_ctrl_t *isp_ctrl = cst_hdl_to_isp_ctrl( hdl );
    int ctx_id = isp_ctrl->ctx_id;

    LOG( LOG_DEBUG, "Control - id:0x%x, val:%d, is_int:%d, min:%d, max:%d.\n",
         ctrl->id, ctrl->val, ctrl->is_int, ctrl->minimum, ctrl->maximum);

    switch ( ctrl->id ) {
    case ISP_V4L2_CID_AE_COMPENSATION:
        LOG( LOG_INFO, "get ae compensation: %d.\n" );
        ctrl->val = fw_intf_get_ae_compensation(ctx_id);
        break;
    case ISP_V4L2_CID_CUSTOM_SNR_MANUAL:
        LOG( LOG_INFO, "get snr manual: %d.\n" );
        ctrl->val = fw_intf_get_custom_snr_manual(ctx_id);
        break;
    case ISP_V4L2_CID_CUSTOM_SNR_STRENGTH:
        LOG( LOG_INFO, "get snr strength: %d.\n" );
        ctrl->val = fw_intf_get_custom_snr_strength(ctx_id);
        break;
    case ISP_V4L2_CID_CUSTOM_TNR_MANUAL:
        LOG( LOG_INFO, "get tnr manual: %d.\n" );
        ctrl->val = fw_intf_get_custom_tnr_manual(ctx_id);
        break;
    case ISP_V4L2_CID_CUSTOM_TNR_OFFSET:
        LOG( LOG_INFO, "get tnr offset: %d.\n" );
        ctrl->val = fw_intf_get_custom_tnr_offset(ctx_id);
        break;
    case ISP_V4L2_CID_CUSTOM_TEMPER_MODE:
        LOG( LOG_INFO, "get temper mode: %d.\n" );
        ctrl->val = fw_intf_get_custom_temper_mode(ctx_id);
        break;
    case ISP_V4L2_CID_CUSTOM_WDR_SWITCH:
        LOG( LOG_INFO, "get wdr mode: %d.\n" );
        ctrl->val = fw_intf_get_customer_sensor_mode(ctx_id);
        break;
    case ISP_V4L2_CID_CUSTOM_ANTIFLICKER:
        LOG( LOG_INFO, "get anti flicker: %d.\n" );
        ctrl->val = fw_intf_get_customer_antiflicker(ctx_id);
        break;
    default:
        ret = 1;
        break;
    }

    return ret;
}

static const struct v4l2_ctrl_ops isp_v4l2_ctrl_ops_custom = {
    .s_ctrl = isp_v4l2_ctrl_s_ctrl_custom,
    .g_volatile_ctrl = isp_v4l2_ctrl_g_ctrl_custom,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_test_pattern = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_TEST_PATTERN,
    .name = "ISP Test Pattern",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 1,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_test_pattern_type = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_TEST_PATTERN_TYPE,
    .name = "ISP Test Pattern Type",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 3,
    .step = 1,
    .def = 3,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_af_refocus = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_AF_REFOCUS,
    .name = "ISP AF Refocus",
    .type = V4L2_CTRL_TYPE_BUTTON,
    .min = 0,
    .max = 0,
    .step = 0,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_preset = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_SENSOR_PRESET,
    .name = "ISP Sensor Preset",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 5,
    .step = 1,
    .def = 0,
};


static const struct v4l2_ctrl_config isp_v4l2_ctrl_af_roi = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_AF_ROI,
    .name = "ISP AF ROI",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 0x7F7F7F7F,
    .step = 1,
    .def = 0x20206060,
};


static const struct v4l2_ctrl_config isp_v4l2_ctrl_output_fr_on_off = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_OUTPUT_FR_ON_OFF,
    .name = "ISP FR ON/OFF",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 0x7FFFFFFF,
    .step = 1,
    .def = 0,
};


static const struct v4l2_ctrl_config isp_v4l2_ctrl_output_ds1_on_off = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_OUTPUT_DS1_ON_OFF,
    .name = "ISP DS1 ON/OFF",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 0x7FFFFFFF,
    .step = 1,
    .def = 0,
};


static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_wdr_mode = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SENSOR_WDR_MODE,
    .name = "ISP Sensor wdr mode",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 3,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_exposure = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SENSOR_EXPOSURE,
    .name = "ISP Sensor exposure",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 1,
    .max = 4,
    .step = 1,
    .def = 1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_fr_fps = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_FR_FPS,
    .name = "ISP fr fps",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 120,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_ds1_fps = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_DS1_FPS,
    .name = "ISP ds1 fps",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 120,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_testpattern = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_SENSOR_TESTPATTERN,
    .name = "ISP Sensor test pattern",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 10,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_ir_cut = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SENSOR_IR_CUT,
    .name = "sensor ir cut set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 2,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_ae_zone_weight = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_AE_ZONE_WEIGHT,
    .name = "set isp ae zone weight",
    .type = V4L2_CTRL_TYPE_INTEGER64,
    .min = 0x8000000000000000,
    .max = 0x7fffffffffffffff,
    .step = 8,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_awb_zone_weight = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_AWB_ZONE_WEIGHT,
    .name = "set isp awb zone weight",
    .type = V4L2_CTRL_TYPE_INTEGER64,
    .min = 0x8000000000000000,
    .max = 0x7fffffffffffffff,
    .step = 8,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_manual_exposure = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_MANUAL_EXPOSURE,
    .name = "manual_exposure set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 2,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_integration_time = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_SENSOR_INTEGRATION_TIME,
    .name = "sensor_integration_timet set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 4000,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_analog_gain = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_SENSOR_ANALOG_GAIN,
    .name = "sensor_analog_gain set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 256,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_isp_digital_gain = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_ISP_DIGITAL_GAIN,
    .name = "isp_digital_gain set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 256,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_stop_sensor_update = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_STOP_SENSOR_UPDATE,
    .name = "stop_sensor_update set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 2,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_ae_compensation = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_AE_COMPENSATION,
    .name = "ISP AE Compensation",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 255,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_digital_gain = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_SENSOR_DIGITAL_GAIN,
    .name = "sensor_digital_gain set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 256,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_awb_red_gain = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_AWB_RED_GAIN,
    .name = "awb_red_gain set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 65535,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_awb_blue_gain = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_AWB_BLUE_GAIN,
    .name = "awb_blue_gain set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 65535,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_max_integration_time = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SET_MAX_INTEGRATION_TIME,
    .name = "max_int_time set",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = -1,
    .max = 5564,
    .step = 1,
    .def = -1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_sensor_fps = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SENSOR_FPS,
    .name = "ISP Sensor fps",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 120,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_snr_manual = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SNR_MANUAL,
    .name = "ISP SNR manual",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 1,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_snr_strength = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_SNR_STRENGTH,
    .name = "ISP SNR strength",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 255,
    .step = 1,
    .def = 128,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_tnr_manual = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_TNR_MANUAL,
    .name = "ISP TNR manual",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 1,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_tnr_offset = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_TNR_OFFSET,
    .name = "ISP TNR offset",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 255,
    .step = 1,
    .def = 128,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_temper_mode = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_TEMPER_MODE,
    .name = "ISP Temper mode",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 1,
    .max = 2,
    .step = 1,
    .def = 1,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_wdr_mode = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_WDR_SWITCH,
    .name = "ISP WDR mode",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 2,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_antiflicker = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_ANTIFLICKER,
    .name = "ISP Anti Flicker mode",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 60,
    .step = 5,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_defog_mode = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_DEFOG_SWITCH,
    .name = "Defog alg mode",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 2,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_defog_ratio = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .id = ISP_V4L2_CID_CUSTOM_DEFOG_STRENGTH,
    .name = "Defog alg ratio delta",
    .type = V4L2_CTRL_TYPE_INTEGER,
    .min = 0,
    .max = 4096,
    .step = 1,
    .def = 0,
};

static const struct v4l2_ctrl_ops isp_v4l2_ctrl_ops = {
    .s_ctrl = isp_v4l2_ctrl_s_ctrl_standard,
    .g_volatile_ctrl = isp_v4l2_ctrl_g_ctrl_standard,
};

static const struct v4l2_ctrl_config isp_v4l2_ctrl_class = {
    .ops = &isp_v4l2_ctrl_ops_custom,
    .flags = V4L2_CTRL_FLAG_READ_ONLY | V4L2_CTRL_FLAG_WRITE_ONLY,
    .id = ISP_V4L2_CID_ISP_V4L2_CLASS,
    .name = "ARM ISP Controls",
    .type = V4L2_CTRL_TYPE_CTRL_CLASS,
};

#define ADD_CTRL_STD( id, min, max, step, def )                  \
    {                                                            \
        if ( fw_intf_validate_control( id ) ) {                  \
            v4l2_ctrl_new_std( hdl_std_ctrl, &isp_v4l2_ctrl_ops, \
                               id, min, max, step, def );        \
        }                                                        \
    }

#define ADD_CTRL_STD_MENU( id, max, skipmask, def )                   \
    {                                                                 \
        if ( fw_intf_validate_control( id ) ) {                       \
            v4l2_ctrl_new_std_menu( hdl_std_ctrl, &isp_v4l2_ctrl_ops, \
                                    id, max, skipmask, def );         \
        }                                                             \
    }

#define ADD_CTRL_CST( id, cfg, priv )                        \
    {                                                        \
        if ( fw_intf_validate_control( id ) ) {              \
            v4l2_ctrl_new_custom( hdl_cst_ctrl, cfg, priv ); \
        }                                                    \
    }

int isp_v4l2_ctrl_init( uint32_t ctx_id, isp_v4l2_ctrl_t *ctrl )
{
    struct v4l2_ctrl_handler *hdl_std_ctrl = &ctrl->ctrl_hdl_std_ctrl;
    struct v4l2_ctrl_handler *hdl_cst_ctrl = &ctrl->ctrl_hdl_cst_ctrl;

    /* Init and add standard controls */
    v4l2_ctrl_handler_init( hdl_std_ctrl, 10 );
    v4l2_ctrl_new_custom( hdl_std_ctrl, &isp_v4l2_ctrl_class, NULL );

    /* general */
    ADD_CTRL_STD( V4L2_CID_BRIGHTNESS, 0, 255, 1, 128 );
    ADD_CTRL_STD( V4L2_CID_CONTRAST, 0, 255, 1, 128 );
    ADD_CTRL_STD( V4L2_CID_SATURATION, 0, 255, 1, 128 );
    ADD_CTRL_STD( V4L2_CID_HUE, 0, 360, 1, 180 );
    ADD_CTRL_STD( V4L2_CID_SHARPNESS, 0, 255, 1, 128 );
    ADD_CTRL_STD_MENU( V4L2_CID_COLORFX, 4, 0x1F0, 0 );
    /* orientation */
    ADD_CTRL_STD( V4L2_CID_HFLIP, 0, 1, 1, 0 );
    ADD_CTRL_STD( V4L2_CID_VFLIP, 0, 1, 1, 0 );
    /* exposure */
    ADD_CTRL_STD( V4L2_CID_AUTOGAIN, 0, 1, 1, 1 );
    ADD_CTRL_STD( V4L2_CID_GAIN, 100, 3200, 1, 100 );
    ADD_CTRL_STD_MENU( V4L2_CID_EXPOSURE_AUTO,
                       1, 0x0, 0 );
    ADD_CTRL_STD( V4L2_CID_EXPOSURE_ABSOLUTE,
                  1, 1000, 1, 33 );
    ADD_CTRL_STD( V4L2_CID_EXPOSURE_AUTO_PRIORITY,
                  0, 1, 1, 0 );
    /* white balance */
    ADD_CTRL_STD( V4L2_CID_AUTO_WHITE_BALANCE,
                  0, 1, 1, 1 );
    ADD_CTRL_STD( V4L2_CID_WHITE_BALANCE_TEMPERATURE,
                  2000, 8000, 1000, 5000 );
    /* focus */
    ADD_CTRL_STD( V4L2_CID_FOCUS_AUTO, 0, 1, 1, 1 );
    ADD_CTRL_STD( V4L2_CID_FOCUS_ABSOLUTE,
                  0, 255, 1, 0 );

    /* Init and add custom controls */
    v4l2_ctrl_handler_init( hdl_cst_ctrl, 2 );
    v4l2_ctrl_new_custom( hdl_cst_ctrl, &isp_v4l2_ctrl_class, NULL );

    ADD_CTRL_CST( ISP_V4L2_CID_TEST_PATTERN,
                  &isp_v4l2_ctrl_test_pattern, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_TEST_PATTERN_TYPE,
                  &isp_v4l2_ctrl_test_pattern_type, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_AF_REFOCUS,
                  &isp_v4l2_ctrl_af_refocus, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_SENSOR_PRESET,
                  &isp_v4l2_ctrl_sensor_preset, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_AF_ROI,
                  &isp_v4l2_ctrl_af_roi, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_OUTPUT_FR_ON_OFF,
                  &isp_v4l2_ctrl_output_fr_on_off, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_OUTPUT_DS1_ON_OFF,
                  &isp_v4l2_ctrl_output_ds1_on_off, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SENSOR_WDR_MODE,
                  &isp_v4l2_ctrl_sensor_wdr_mode, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SENSOR_EXPOSURE,
                  &isp_v4l2_ctrl_sensor_exposure, NULL );
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_FR_FPS,
                  &isp_v4l2_ctrl_fr_fps, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_SENSOR_TESTPATTERN,
                  &isp_v4l2_ctrl_sensor_testpattern, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SENSOR_IR_CUT,
                  &isp_v4l2_ctrl_sensor_ir_cut, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_AE_ZONE_WEIGHT,
                  &isp_v4l2_ctrl_ae_zone_weight, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_AWB_ZONE_WEIGHT,
                  &isp_v4l2_ctrl_awb_zone_weight, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_MANUAL_EXPOSURE,
                  &isp_v4l2_ctrl_manual_exposure, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_SENSOR_INTEGRATION_TIME,
                  &isp_v4l2_ctrl_sensor_integration_time, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_SENSOR_ANALOG_GAIN,
                  &isp_v4l2_ctrl_sensor_analog_gain, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_ISP_DIGITAL_GAIN,
                  &isp_v4l2_ctrl_isp_digital_gain, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_STOP_SENSOR_UPDATE,
                  &isp_v4l2_ctrl_stop_sensor_update, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_DS1_FPS,
                  &isp_v4l2_ctrl_ds1_fps, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_AE_COMPENSATION,
                  &isp_v4l2_ctrl_ae_compensation, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_SENSOR_DIGITAL_GAIN,
                  &isp_v4l2_ctrl_sensor_digital_gain, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_AWB_RED_GAIN,
                  &isp_v4l2_ctrl_awb_red_gain, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_AWB_BLUE_GAIN,
                  &isp_v4l2_ctrl_awb_blue_gain, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SET_MAX_INTEGRATION_TIME,
                  &isp_v4l2_ctrl_max_integration_time, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SENSOR_FPS,
                  &isp_v4l2_ctrl_sensor_fps, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SNR_MANUAL,
                  &isp_v4l2_ctrl_snr_manual, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_SNR_STRENGTH,
                  &isp_v4l2_ctrl_snr_strength, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_TNR_MANUAL,
                  &isp_v4l2_ctrl_tnr_manual, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_TNR_OFFSET,
                  &isp_v4l2_ctrl_tnr_offset, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_TEMPER_MODE,
                  &isp_v4l2_ctrl_temper_mode, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_WDR_SWITCH,
                  &isp_v4l2_ctrl_wdr_mode, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_ANTIFLICKER,
                  &isp_v4l2_ctrl_antiflicker, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_DEFOG_SWITCH,
                  &isp_v4l2_ctrl_defog_mode, NULL);
    ADD_CTRL_CST( ISP_V4L2_CID_CUSTOM_DEFOG_STRENGTH,
                  &isp_v4l2_ctrl_defog_ratio, NULL);
    /* Add control handler to v4l2 device */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0))
    v4l2_ctrl_add_handler( hdl_std_ctrl, hdl_cst_ctrl, NULL );
#else
    v4l2_ctrl_add_handler( hdl_std_ctrl, hdl_cst_ctrl, NULL, false );
#endif
    ctrl->video_dev->ctrl_handler = hdl_std_ctrl;

    v4l2_ctrl_handler_setup( hdl_std_ctrl );

    return 0;
}

void isp_v4l2_ctrl_deinit( isp_v4l2_ctrl_t *ctrl )
{
    v4l2_ctrl_handler_free( &ctrl->ctrl_hdl_std_ctrl );
    v4l2_ctrl_handler_free( &ctrl->ctrl_hdl_cst_ctrl );
}
