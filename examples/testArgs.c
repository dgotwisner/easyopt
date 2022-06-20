/* testArgs.c
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

#include <stdio.h>

#include "easyopts.h"

struct opts
{
    struct A {
        char *sa0;
        char *sa1;
        int ia2;
    } a;
    struct B {
        char *sb0;
        char *sb1;
        float fb2;
    } b;
    struct C {
        double dc0;
        double dc1;
        int ic2;
    } c;
    struct D {
        double dd0;
        double dd1;
        float fd2;
    } d;
};

#include "testArgsAssignFcns.inc"
#include "testArgsValidateFcns.inc"

#define dim(x) (sizeof(x) / sizeof(x[0]))

#ifdef notdef
void easyopts_addOption(void *gs,
        char shortOption,
        const char *longOption,
        easyopts_dataTypeEnum_t type,
        /*void *defaultValue,*/
        easyopts_required_t isRequired,
        void (*validate)(easyopts_dataType_t *),
        void (*assign)(easyopts_dataType_t *, void *),
        const char *description)
typedef enum easyopts_dataTypeEnum
{
    DATATYPE_INVALID = 0,
    DATATYPE_SIGNED_CHAR,
    DATATYPE_UNSIGNED_CHAR,
    DATATYPE_SIGNED_SHORT,
    DATATYPE_UNSIGNED_SHORT,
    DATATYPE_SIGNED_INT,
    DATATYPE_UNSIGNED_INT,
    DATATYPE_SIGNED_LONG,
    DATATYPE_UNSIGNED_LONG,
    DATATYPE_SIGNED_LONG_LONG,
    DATATYPE_UNSIGNED_LONG_LONG,
    DATATYPE_FLOAT,
    DATATYPE_DOUBLE,
    DATATYPE_STRING,
    DATATYPE_LIMIT = DATATYPE_STRING + 1
} easyopts_dataTypeEnum_t;
#endif // notdef

int main(int ac, char **av)
{
    void *sect;
    easyopts_remainingArgs_t *remainder = NULL;
    easyopts_initProgramOptions(ac, av, "This is the general description of this test program");
    sect = easyopts_addSection( "Section A", "This is the first test section, it should have 3 options: two strings and an integer", TYPE_PUBLIC     );
        easyopts_addOption(sect, 0, "argA0",  DATATYPE_STRING,     /* "foo", */ REQUIRED_REQUIRED, validateSA0, assignSA0, "Section A, argument 0 description");
        easyopts_addOption(sect, 0, "argA1",  DATATYPE_STRING,     /* "bar", */ REQUIRED_OPTIONAL, validateSA1, assignSA1, "Section A, argument 1 description");
        easyopts_addOption(sect, 0, "iargA2", DATATYPE_SIGNED_INT, /* 12, */    REQUIRED_OPTIONAL, validateIA2, assignIA2, "Section A, argument 2 description");
    sect = easyopts_addSection( "Section B", "This is the second test section, it should have 2 options: two strings and a float",   TYPE_DEPRECATED );
        easyopts_addOption(sect, 0, "argB0",  DATATYPE_STRING,     /* "foo", */ REQUIRED_REQUIRED, validateSB0, assignSB0, "Section B, argument 0 description");
        easyopts_addOption(sect, 0, "argB1",  DATATYPE_STRING,     /* "bar", */ REQUIRED_OPTIONAL, validateSB1, assignSB1, "Section B, argument 1 description");
        easyopts_addOption(sect, 0, "iargB2", DATATYPE_FLOAT,      /* 3.14, */  REQUIRED_OPTIONAL, validateFB2, assignFB2, "Section B, argument 2 description");
    sect = easyopts_addSection( "Section C", "This is the third test section, it should have 2 options: two doubles and an integer", TYPE_HIDDEN     );
        easyopts_addOption(sect, 0, "argC0",  DATATYPE_DOUBLE,     /* 1.234, */ REQUIRED_REQUIRED, validateDC0, assignDC0, "Section C, argument 0 description");
        easyopts_addOption(sect, 0, "argC1",  DATATYPE_DOUBLE,     /* 5.678, */ REQUIRED_OPTIONAL, validateDC1, assignDC1, "Section C, argument 1 description");
        easyopts_addOption(sect, 0, "iargC2", DATATYPE_SIGNED_INT, /* 12, */    REQUIRED_OPTIONAL, validateIC2, assignIC2, "Section C, argument 2 description");
    sect = easyopts_addSection( "Section D", "This is the fourth test section, it should have 2 options: two doubles and a float",   TYPE_PUBLIC     );
        easyopts_addOption(sect, 0, "argD0",  DATATYPE_DOUBLE,     /* 1.234, */ REQUIRED_REQUIRED, validateDD0, assignDD0, "Section D, argument 0 description");
        easyopts_addOption(sect, 0, "argD1",  DATATYPE_DOUBLE,     /* 5.678, */ REQUIRED_OPTIONAL, validateDD1, assignDD1, "Section D, argument 1 description");
        easyopts_addOption(sect, 0, "iargD2", DATATYPE_FLOAT,      /* 9.012, */ REQUIRED_OPTIONAL, validateFD2, assignFD2, "Section D, argument 2 description");

    printf("========================================\n");
    printf("HELP\n");
    easyopts_help();
    printf("========================================\n");
    printf("HELP + HIDDEN\n");
    easyopts_help_hidden();
    printf("========================================\n");
    printf("JSON HELP\n");
    easyopts_help_json();
    printf("========================================\n");
    printf("JSON HELP + JSON\n");
    easyopts_help_hidden_json();
    printf("========================================\n");

    easyopts_free(remainder);
    printf("Version: %s\n", easyopts_getVersion());

#if 0
    easyopts_programOptions_t program;
    
    easyopts_section_t sectionA;
    easyopts_section_t sectionB;
    easyopts_section_t sectionC;

    easyopts_section_t tmpSection; /* used for adding items to a section */

    static easyopts_section_t sections[] = {
    };

    static easyopts_item_t itemsA[] = {
    };
#if 0
    static easyopts_item_t itemsB[] = {
        { 0, "argB0",  "Section B, argument 0 description", "foo", REQUIRED_OPTIONAL, NULL,  assignB0 },
        { 0, "argB1",  "Section B, argument 1 description", "bar", REQUIRED_OPTIONAL, NULL,  assignB1 },
    };
    static easyopts_item_t itemsC[] = {
        { 0, "dargC0", "Section C, argument 0 description",  3.14, REQUIRED_OPTIONAL, NULL,  assignC0 },
        { 0, "dargC1", "Section C, argument 1 description", 12.50, REQUIRED_OPTIONAL, NULL,  assignC1 },
    };

    /* This must be in the same order as the sections array */
    easyopts_item_t *itemToSectionArray[] = {
        &itemsA,
        &itemsB,
        &itemsC
    };
#endif // 0
    // section A: --argA0=foo --argA1=bar --iargA2=12
    // section B: --argB0=ralph --argB1=most
#endif // 0
}
