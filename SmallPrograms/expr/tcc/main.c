#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <libtcc.h>

void tcc_error_callback(void *opaque, const char *msg) {
    fprintf(opaque, "%s\n", msg);
}

char *code_head = 
"#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include <math.h>\n\
double primary() {\n\
    return ( ";

char *code_end = 
" );\n\
}";

int main(int argc, char **argv)
{
    if(argc!=2) {
        puts("arguments error!");
        exit(0);
    }
    char *code = (char *)malloc(strlen(code_head)+strlen(code_end)+strlen(argv[1])+1);
    strcpy(code, code_head);
    strcat(code, argv[1]);
    strcat(code, code_end);
    
    TCCState *s;
    double (*primary)(void);
    s = tcc_new();
    if (!s) {
        fprintf(stderr, "Could not create tcc state\n");
        exit(1);
    }
    
    assert(tcc_get_error_func(s) == NULL);
    assert(tcc_get_error_opaque(s) == NULL);

    tcc_set_error_func(s, stderr, tcc_error_callback);

    assert(tcc_get_error_func(s) == tcc_error_callback);
    assert(tcc_get_error_opaque(s) == stderr);
    
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    
    if (tcc_compile_string(s, code) == -1)
        return 1;

    /* relocate the code */
    if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0)
        return 1;

    /* get entry symbol */
    primary = tcc_get_symbol(s, "primary");
    if (!primary)
        return 1;

    /* run the code */
    double retval = primary();
    printf("%g\n", retval);
    
    /* delete the state */
    tcc_delete(s);

    return 0;
}
