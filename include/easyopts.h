/* easyopts.h
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

/* Easyopts provides a different way to do command line arguments.
 *
 * It is C code, not C++, and works by registration, so you call the
 * function with the structure containing your command line stuff, and,
 * using callbacks, when it is returned, a structure containing the argument
 * values is returned.
 *
 * In one sense, it's similar to boost's command line, in that the usage
 * information, data types, long and short options are all tied together,
 * but, again, it's C not C++.
 *
 * This library is Linux only, at this point.
 *
 * The data structure consists of a list of sections, each section has a set
 * of command line options.  The section also has a section name, a section
 * description, and a type (public, hidden, deprecated).
 *
 * TODO: Format the columns and data so they wrap correctly based upon the
 * terminal width, and options indent to form consistent columns.
 */

#pragma once

typedef union easyopts_dataType
{
    signed char sc;
    unsigned char uc;
    signed short ss;
    unsigned short us;
    signed int si;
    unsigned int ui;
    signed long sl;
    unsigned long ul;
    signed long long sll;
    unsigned long long ull;
    float f;
    double d;
    /* These aren't supported by the compiler I use
    signed __int128 si128;
    unsigned __int128 ui128;
    */
    char *strData;
} easyopts_dataType_t;

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

typedef enum easyopts_type {
    TYPE_INVALID = 0,
    TYPE_PUBLIC,
    TYPE_HIDDEN,
    TYPE_DEPRECATED,
    TYPE_LIMIT = TYPE_DEPRECATED + 1
} easyopts_type_t;

typedef enum required {
    REQUIRED_INVALID = 0,
    REQUIRED_NONE,
    REQUIRED_REQUIRED,
    REQUIRED_OPTIONAL,
    REQUIRED_LIMIT = REQUIRED_OPTIONAL + 1
} easyopts_required_t;

typedef struct easyopts_remainingArgs
{
    int remainingArgsSize;
    char **remainingArgs;
} easyopts_remainingArgs_t;

/* This MUST be called first.  It initializes the internal structure, and
 * binds a description and the command line argc/argv to the program
 */
extern void easyopts_initProgramOptions(int argc, char *argv[], const char *description);

/* Add a section to a program.  Returns a handle to the section, so it can be used to add items.
 */
extern void *easyopts_addSection(const char *name, const char *description, easyopts_type_t type);

/* Add an item to a section
 */
/* Note: the default value is actually a easyopts_dataTypeEnum_t and is internally cast.
 *
 * The validate() function gets called if this option is provided on the command line.  It fills the appropriate union element in type with the value on the command
 * line.  If no argument is provided, the validation function is not called.  The provider of the callback should provide the business logic to do validation.
 * Return 0 on failure, 1 on success.  Validation occurs in the order of option registration.  If multiple options have interactions, the validation should be put in
 * the validation function for the last option verified, but the earlier ones will need to save the data somewhere for access by the later callback function.
 *
 * The assign() function will provide the same data in type that was provided to the validate() function (it is called right after all validation of all options
 * is performed).  The options field is provided as a place to fill in the expected value based upon business rules.  For something like --help, assign will print the
 * help message and exit.  Assign() functions are executed in the order of registration.
 */
extern void easyopts_addOption(void *gs, char shortOption, const char *longOption, easyopts_dataTypeEnum_t type,
    /*void *defaultValue,*/ easyopts_required_t isRequired, int (*validate)(easyopts_dataType_t *type),
    void (*assign)(easyopts_dataType_t *type, void *options), const char *description);

#if 0
/* Process the command line, returns < 0 on error, 0 on success.
 *
 * argc, argv are the arguments to main().
 * gpo is the set of program options built up by calling easyopts_addSection() and easyopts_addOption(), as appropriate.
 * gr is a pointer.  This function will allocate the approprate easyopts_remainingArgs with every argument not processed by registration.
 */
extern int easyopts_process(int argc, char **argv, easyopts_programOptions_t *gpo, easyopts_remainingArgs_t *gra);
#endif // 0

/* When done, free up all the easyopts_programOptions and easyopts_remainingArgs data
 */
extern void easyopts_free(easyopts_remainingArgs_t *gra);

extern void easyopts_help(void);
extern void easyopts_help_hidden(void);
extern void easyopts_help_json(void);
extern void easyopts_help_hidden_json(void);
extern const char *easyopts_getVersion(void);
