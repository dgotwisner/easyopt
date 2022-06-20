/* testArgsAssignFcns.inc
 *
 * BSD 3-Clause License
 * 
 * Copyright (c) 2022 David I Gotwisner
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <stdio.h>

void assignSA0(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->a.sa0 = v_value->strData;
    printf("Setting a.sa0 to '%s'\n", v_value->strData);
}

void assignSA1(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->a.sa1 = v_value->strData;
    printf("Setting a.sa1 to '%s'\n", v_value->strData);
}

void assignIA2(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->a.ia2 = v_value->si;
    printf("Setting a.ia2 to %d\n", v_value->si);
}

void assignSB0(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->b.sb0 = v_value->strData;
    printf("Setting b.b0 to '%s'\n", v_value->strData);
}

void assignSB1(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->b.sb1 = v_value->strData;
    printf("Setting b.b1 to '%s'\n", v_value->strData);
}

void assignFB2(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->b.fb2 = v_value->f;
    printf("Setting b.fb2 to %d\n", v_value->f);
}

void assignDC0(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->c.dc0 = v_value->d;
    printf("Setting c.dc0 to %f\n", v_value->d);
}

void assignDC1(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->c.dc1 = v_value->d;
    printf("Setting c.dc1 to %f\n", v_value->d);
}

void assignIC2(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->c.ic2 = v_value->d;
    printf("Setting c.ic2 to %f\n", v_value->si);
}

void assignDD0(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->d.dd0 = v_value->d;
    printf("Setting d.dd0 to %f\n", v_value->d);
}

void assignDD1(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->d.dd1 = v_value->d;
    printf("Setting d.dd1 to %f\n", v_value->d);
}

void assignFD2(easyopts_dataType_t *v_value, void *v_object)
{
    struct opts *o = (struct opts *)v_object;

    o->d.fd2 = v_value->d;
    printf("Setting d.fd2 to %f\n", v_value->f);
}
