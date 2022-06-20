/* easyopts.c
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

#include <stdlib.h>
#include <stdio.h>

#include "easyopts.h"
#include "defs.h"

typedef struct easyopts_sections_list easyopts_sections_list_t;
typedef struct easyopts_options_list easyopts_options_list_t;
typedef struct easyopts_section easyopts_section_t;
typedef struct easyopts_option easyopts_option_t;

struct easyopts_option
{
    char shortOption;
    const char *longOption;
    easyopts_dataTypeEnum_t type;
    //????? defaultValue;
    easyopts_required_t isRequired;
    int (*validate)(easyopts_dataType_t *value);
    void (*assign)(easyopts_dataType_t *value, void *storageObject);
    const char *description;
};

struct easyopts_section
{
    const char *name;
    const char *description;
    easyopts_type_t type;
    easyopts_options_list_t *firstOption;
    easyopts_options_list_t *lastOption;
};

// Linked list of sections
struct easyopts_sections_list
{
    easyopts_section_t *object;
    easyopts_sections_list_t *next;
};

// Linked list of options
struct easyopts_options_list
{
    easyopts_option_t *object;
    easyopts_options_list_t *next;
};

/* This is the program options structure.  We use it internally, it doesn't
 * need to be exposed.
 */
typedef struct easyopts_programOptions
{
    // Filled in by easyopts_initProgramOptions()
    int argc;
    char **argv;
    const char *description;

    // Filled in by easyopts_addSection
    easyopts_sections_list_t *firstSection; // Pointer to first section, output from here, to preserve ordering
    easyopts_sections_list_t *lastSection; // Pointer to last section, insert here, to preserve ordering

} easyopts_programOptions_t;

/* There will only be one of these per program, and the user doesn't need to see it */
static easyopts_programOptions_t s_commandLineOptions;

static const char *version = NULL;

void assignVersion(easyopts_dataType_t *v_value, void *v_object)
{
    version = easyopts_getVersion();
    printf("Easyopts Version %s\n", version);
}

void assignHelp(easyopts_dataType_t *v_value, void *v_object)
{
    // Print help and then exit
    easyopts_help();
    exit(0);
}

void assignHelpHidden(easyopts_dataType_t *v_value, void *v_object)
{
    // Print help (with hidden) and then exit
    easyopts_help_hidden();
    exit(0);
}

void assignHelpJson(easyopts_dataType_t *v_value, void *v_object)
{
}

void assignHelpHiddenJson(easyopts_dataType_t *v_value, void *v_object)
{
}

void easyopts_initProgramOptions(int argc, char *argv[], const char *description)
{
    s_commandLineOptions.argc = argc;
    s_commandLineOptions.argv = argv;
    s_commandLineOptions.description = description;
    s_commandLineOptions.firstSection = NULL;
    s_commandLineOptions.lastSection = NULL;

    // And now add the default groups with the help and version options
    void *sect;
    sect = easyopts_addSection( "Common", "Provide Common Arguments for help and versioning", TYPE_PUBLIC     );
        easyopts_addOption(sect, 'v', "version",           DATATYPE_STRING,     /* "foo", */ REQUIRED_NONE, NULL, assignVersion, "Print the library's version information");
        easyopts_addOption(sect, 'h', "help",              DATATYPE_STRING,     /* "foo", */ REQUIRED_NONE, NULL, assignHelp, "Print program usage and exit.");
        easyopts_addOption(sect, 0,   "help-json",         DATATYPE_STRING,     /* "foo", */ REQUIRED_NONE, NULL, assignHelpJson, "Print program usage in Json format and exit.");
    sect = easyopts_addSection( "Common Hidden", "Provide Common Arguments for help and versioning (Hidden)", TYPE_HIDDEN     );
        easyopts_addOption(sect, 0,   "help-hidden",       DATATYPE_STRING,     /* "foo", */ REQUIRED_NONE, NULL, assignHelpHidden, "Print program usage (including hidden options) and exit.");
        easyopts_addOption(sect, 0,   "help-hidden-json",  DATATYPE_STRING,     /* "foo", */ REQUIRED_NONE, NULL, assignHelpHiddenJson, "Print program usage in Json format (including hidden options) and exit.");
}

static void freeOption(easyopts_options_list_t *option)
{
    free(option->object); // free option object associated with list item
    free(option); // and free the list item
}

static void freeSection(easyopts_sections_list_t *section)
{
    easyopts_options_list_t *cur;
    while(section->object->firstOption != NULL) {
        cur = section->object->firstOption;
        section->object->firstOption = section->object->firstOption->next;
        freeOption(cur);
    }
    free(section->object); // delete the section object associated with the list item
    free(section); // delete the list item for the section
}

void easyopts_free(easyopts_remainingArgs_t *gra)
{
    /* Free the program options */
    easyopts_sections_list_t *cur;
    while(s_commandLineOptions.firstSection != NULL) {
        cur = s_commandLineOptions.firstSection;
        s_commandLineOptions.firstSection = s_commandLineOptions.firstSection->next;
        freeSection(cur);
    }

    /* And now free the remainingArgs */
    if (gra != NULL) {
        int i;
        for (i = 0; i < gra->remainingArgsSize; i++) {
            free(gra->remainingArgs[i]);
            gra->remainingArgs[i] = NULL; /* protect memory */
        }
        free(gra->remainingArgs);
        gra->remainingArgs = NULL; /* protect memory */
        free(gra);
    }
}

/* Add gs to gpo.  We will sort when we are done, so ordering doesn't matter */
void *easyopts_addSection(const char *name, const char *description, easyopts_type_t type)
{
    easyopts_section_t *section = (easyopts_section_t *)malloc(sizeof(easyopts_section_t));
    section->name = name;
    section->description = description;
    section->type = type;
    section->firstOption = NULL;
    section->lastOption = NULL;

    // Allocate a sections_list item to hold it.
    easyopts_sections_list_t *item = (easyopts_sections_list_t *)malloc(sizeof(easyopts_sections_list_t));
    item->object = section;
    item->next = NULL;

    // And link it in
    if (s_commandLineOptions.firstSection == NULL) {
        s_commandLineOptions.firstSection = item;
    }
    if (s_commandLineOptions.lastSection != NULL) {
        s_commandLineOptions.lastSection->next = item;
    }
    s_commandLineOptions.lastSection = item;
    return (void *)section;
}

// Note: gs is a easyopts_section_t, but it is not exposed to the header
void easyopts_addOption(void *gs, char shortOption, const char *longOption,
    easyopts_dataTypeEnum_t type, /*void *defaultValue,*/
    easyopts_required_t isRequired,
    int (*validate)(easyopts_dataType_t *type),
    void (*assign)(easyopts_dataType_t *type, void *options),
    const char *description)
{
    easyopts_section_t *section = (easyopts_section_t *)gs;

    // Create and fill in the option object
    easyopts_option_t *option = (easyopts_option_t *)malloc(sizeof(easyopts_option_t));
    option->shortOption = shortOption;
    option->longOption = longOption;
    option->type = type;
    //TODO:option->defaultValue = defaultValue;
    option->isRequired = isRequired;
    option->validate = validate;
    option->assign = assign;
    option->description = description;

    // Create a list node and bind the option data to it
    easyopts_options_list_t *pListItem = (easyopts_options_list_t *)malloc(sizeof(easyopts_options_list_t));
    pListItem->object = option;

    // And connect the node to the end
    if (section->firstOption == NULL) {
        section->firstOption = pListItem;
    }
    if (section->lastOption != NULL) {
        section->lastOption->next = pListItem;
    }
    section->lastOption = pListItem;
}

#if 0
int easyopts_process(int argc, char **argv, easyopts_programOptions_t *gpo, easyopts_remainingArgs_t *gra)
{
}

#endif // 0

static const char *easyopts_print_type(easyopts_type_t t)
{
    switch(t) {
        case TYPE_PUBLIC: return "Options are public";
        case TYPE_HIDDEN: return "Options are hidden";
        case TYPE_DEPRECATED: return "Options are deprecated";
    }
    return "Options are Unknown";
}


static const char *easyopts_print_option_type(easyopts_dataTypeEnum_t t)
{
    switch(t) {
        case DATATYPE_INVALID: return "Invalid";
        case DATATYPE_SIGNED_CHAR: return "Signed Char";
        case DATATYPE_UNSIGNED_CHAR: return "Unsigned Char";
        case DATATYPE_SIGNED_SHORT: return "Signed Short";
        case DATATYPE_UNSIGNED_SHORT: return "Unsigned Short";
        case DATATYPE_SIGNED_INT: return "Signed Integer";
        case DATATYPE_UNSIGNED_INT: return "Unsigned Integer";
        case DATATYPE_SIGNED_LONG: return "Signed Long";
        case DATATYPE_UNSIGNED_LONG: return "Unsigned Long";
        case DATATYPE_SIGNED_LONG_LONG: return "Signed Long Long";
        case DATATYPE_UNSIGNED_LONG_LONG: return "Unsigned Long Long";
        case DATATYPE_FLOAT: return "Float";
        case DATATYPE_DOUBLE: return "Double";
        case DATATYPE_STRING: return "String";
    }
    return "Options are Unknown";
}

/* TODO: Format based upon terminal size */
static void _easyopts_help(int showHidden)
{
    // TODO: Deal with --version and --help (for both visible and hidden
    easyopts_sections_list_t *sections = s_commandLineOptions.firstSection;
    easyopts_sections_list_t *sect;
    easyopts_options_list_t *option;

    printf("%s\n\n", s_commandLineOptions.description);
    printf("Usage: %s", s_commandLineOptions.argv[0]);

    // Do the command line, all on one line
    for (sect = sections; sect != NULL; sect = sect->next) {
        easyopts_section_t *s = sect->object;
        int printIt = 0;
        if (showHidden) {
            printIt = 1;
        } else {
            if (s->type != TYPE_HIDDEN) {
                printIt = 1;
            }
        }
        if (printIt) {
            for (option = s->firstOption; option != NULL; option = option->next) {
                easyopts_option_t *i = ((easyopts_option_t *) option->object);
                if (i->isRequired == REQUIRED_REQUIRED) {
                    printf(" --%s=X", i->longOption);
                } else {
                    printf(" [--%s=X]", i->longOption);
                }
            }
        }
    }

    // TODO: Also list short options for the above

    printf("\n");

    // And now print each section options with descriptive text
    // TODO: Make hidden options truly hidden
    int maxLongLength = 0;
    for (sect = sections; sect != NULL; sect = sect->next) {
        easyopts_section_t *s = sect->object;
        int printIt = 0;
        if (showHidden) {
            printIt = 1;
        } else {
            if (s->type != TYPE_HIDDEN) {
                printIt = 1;
            }
        }
        if (printIt) {
            printf("[%s: %s]\n", s->name, easyopts_print_type(s->type));
            printf("%s\n", s->description);
            for (option = s->firstOption; option != NULL; option = option->next) {
                easyopts_option_t *i = ((easyopts_option_t *) option->object);
                printf("--%s=[%s] %s\n",
                    i->longOption, easyopts_print_option_type(i->type), i->description);
                // TODO: Fill this in: "  -short, --long=[,add default] description
                // TODO: Indicate if it's hidden or deprecated
            }
            printf("\n");
        }
    }
}

void easyopts_help(void)
{
    _easyopts_help(0);
}

void easyopts_help_hidden(void)
{
    _easyopts_help(1);
}

// TODO: Make sure escapes and quotes are dealt with correctly
static void _easyopts_help_json(int showHidden)
{
#if 0
    // TODO: Deal with --version and --help (for both visible and hidden
    easyopts_sections_list_t *sections = s_commandLineOptions.firstSection;
    easyopts_sections_list_t *sect;
    easyopts_options_list_t *option;

    printf("%s\n\n", s_commandLineOptions.description);
    printf("Usage: %s", s_commandLineOptions.argv[0]);

    // Do the command line, all on one line
    for (sect = sections; sect != NULL; sect = sect->next) {
        easyopts_section_t *s = sect->object;
        for (option = s->firstOption; option != NULL; option = option->next) {
            easyopts_option_t *i = ((easyopts_option_t *) option->object);
            if (i->isRequired == REQUIRED_REQUIRED) {
                printf(" --%s=X", i->longOption);
            } else {
                printf(" [--%s=X]", i->longOption);
            }
        }
    }

    printf("\n");

    // And now print each section options with descriptive text
    // TODO: Make hidden options truly hidden
    int maxLongLength = 0;
    for (sect = sections; sect != NULL; sect = sect->next) {
        easyopts_section_t *s = sect->object;
        printf("[%s: %s]\n", s->name, easyopts_print_type(s->type));
        printf("%s\n", s->description);
        for (option = s->firstOption; option != NULL; option = option->next) {
            easyopts_option_t *i = ((easyopts_option_t *) option->object);
            printf("--%s=[%s] %s\n",
                i->longOption, easyopts_print_option_type(i->type), i->description);
            // TODO: Fill this in: "  -short, --long=[,add default] description
            // TODO: Indicate if it's hidden or deprecated
        }
        printf("\n");
    }
#endif // 0
}


void easyopts_help_json(void)
{
    _easyopts_help_json(0);
}

void easyopts_help_hidden_json(void)
{
    _easyopts_help_json(1);
}

const char *easyopts_getVersion(void)
{
    return VERSION;
}
