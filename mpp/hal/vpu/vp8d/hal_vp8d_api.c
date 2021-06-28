/*
*
* Copyright 2015 Rockchip Electronics Co. LTD
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#define MODULE_TAG "hal_vp8d_api"

#include <string.h>

#include "rk_type.h"
#include "mpp_hal.h"
#include "mpp_platform.h"
#include "hal_vp8d_vdpu1.h"
#include "hal_vp8d_vdpu2.h"

static MPP_RET hal_vp8d_reg_gen (void *hal, HalTaskInfo *task)
{
    VP8DHalContext_t *self = (VP8DHalContext_t *)hal;
    return self->hal_api.reg_gen (hal, task);
}

static MPP_RET hal_vp8d_start (void *hal, HalTaskInfo *task)
{
    VP8DHalContext_t *self = (VP8DHalContext_t *)hal;
    return self->hal_api.start (hal, task);
}

static MPP_RET hal_vp8d_wait (void *hal, HalTaskInfo *task)
{
    VP8DHalContext_t *self = (VP8DHalContext_t *)hal;
    return self->hal_api.wait (hal, task);
}

static MPP_RET hal_vp8d_reset (void *hal)
{
    VP8DHalContext_t *self = (VP8DHalContext_t *)hal;
    return self->hal_api.reset (hal);
}

static MPP_RET hal_vp8d_flush (void *hal)
{
    VP8DHalContext_t *self = (VP8DHalContext_t *)hal;
    return self->hal_api.flush (hal);
}

static MPP_RET hal_vp8d_control (void *hal, RK_S32 cmd_type, void *param)
{
    VP8DHalContext_t *self = (VP8DHalContext_t *)hal;
    return self->hal_api.control (hal, cmd_type, param);
}

static MPP_RET hal_vp8d_deinit (void *hal)
{
    VP8DHalContext_t *self = (VP8DHalContext_t *)hal;
    return self->hal_api.deinit (hal);
}

static MPP_RET hal_vp8d_init (void *hal, MppHalCfg *cfg)
{
    VP8DHalContext_t *self = (VP8DHalContext_t *)hal;
    MppHalApi *p_api = NULL;
    VpuHardMode hard_mode = MODE_NULL;
    RK_U32 hw_flag = 0;

    if (NULL == self)
        return MPP_ERR_VALUE;
    memset(self, 0, sizeof(VP8DHalContext_t));

    p_api = &self->hal_api;

    hw_flag = mpp_get_vcodec_type();
    if (hw_flag & HAVE_VDPU1)
        hard_mode = VDPU1_MODE;
    if (hw_flag & HAVE_VDPU2)
        hard_mode = VDPU2_MODE;

    switch (hard_mode) {
    case VDPU2_MODE:
        p_api->init = hal_vp8d_vdpu2_init;
        p_api->deinit = hal_vp8d_vdpu2_deinit;
        p_api->reg_gen = hal_vp8d_vdpu2_gen_regs;
        p_api->start = hal_vp8d_vdpu2_start;
        p_api->wait = hal_vp8d_vdpu2_wait;
        p_api->reset = hal_vp8d_vdpu2_reset;
        p_api->flush = hal_vp8d_vdpu2_flush;
        p_api->control = hal_vp8d_vdpu2_control;
        break;
    case VDPU1_MODE:
        p_api->init = hal_vp8d_vdpu1_init;
        p_api->deinit = hal_vp8d_vdpu1_deinit;
        p_api->reg_gen = hal_vp8d_vdpu1_gen_regs;
        p_api->start = hal_vp8d_vdpu1_start;
        p_api->wait = hal_vp8d_vdpu1_wait;
        p_api->reset = hal_vp8d_vdpu1_reset;
        p_api->flush = hal_vp8d_vdpu1_flush;
        p_api->control = hal_vp8d_vdpu1_control;
        break;
    default:
        return MPP_ERR_INIT;
        break;
    }

    return p_api->init (hal, cfg);
}


const MppHalApi hal_api_vp8d = {
    .name = "vp8d_rkdec",
    .type = MPP_CTX_DEC,
    .coding = MPP_VIDEO_CodingVP8,
    .ctx_size = sizeof(VP8DHalContext_t),
    .flag = 0,
    .init = hal_vp8d_init,
    .deinit = hal_vp8d_deinit,
    .reg_gen = hal_vp8d_reg_gen,
    .start = hal_vp8d_start,
    .wait = hal_vp8d_wait,
    .reset = hal_vp8d_reset,
    .flush = hal_vp8d_flush,
    .control = hal_vp8d_control,
};
