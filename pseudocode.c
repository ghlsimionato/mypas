/**@<pseudocode.c>::**/


#include <pseudocode.h>

void negate(int type)
{
		if (semantic_error) return;

    switch(type) {
        case BOOL:
            printf("\tnotb accb\n");
            break;
        case INT32:
            printf("\tnegl accl\n");
            break;
        case INT64:
            printf("\tnegq accq\n");
            break;
        case FLT32:
            printf("\tnegf accf\n");
            break;
        case FLT64:
            printf("\tnegdf accdf\n");
            break;
    }
}

void R_value(int var_type, const char *name) {
	if (semantic_error) return;

	switch(var_type) {
	case 2:/** int32 **/
		printf("\tmovl %s, accl\n", name);
		break;
	case 3:/** int64 **/
		printf("\tmovq %s, accq\n", name);
		break;
	case 4:/** flt32 **/
		printf("\tmovf %s, accf\n", name);
		break;
	case 5:/** flt64 **/
		printf("\tmovdf %s, accdf\n", name);
		break;
	default:
		;
	}
}
void L_value(int var_type, const char *name) {
	if (semantic_error) return;

	switch(var_type) {
	case 2:/** int32 **/
		printf("\tmovl accl, %s\n", name);
		break;
	case 3:/** int64 **/
		printf("\tmovq accq, %s\n", name);
		break;
	case 4:/** flt32 **/
		printf("\tmovf accf, %s\n", name);
		break;
	case 5:/** flt64 **/
		printf("\tmovdf acdf, %s\n", name);
		break;
	default:
		;
	}
}
void add(int type) {
	if (semantic_error) return;

	switch(type) {
	case 2:/** int32 **/
		printf("\tpopl regl\n");
		printf("\taddl regl, accl\n");
		break;
	case 3:/** int64 **/
		printf("\tpopq regq\n");
		printf("\taddq regq, accq\n");
		break;
	case 4:/** flt32 **/
		printf("\tpopf regf\n");
		printf("\taddf regf, accf\n");
		break;
	case 5:/** flt64 **/
		printf("\tpopdf regdf\n");
		printf("\tadddf regdf, accdf\n");
		break;
	default:
		;
	}
}

void divide(int type) {
	if (semantic_error) return;

    switch(type) {
        case INT32:
            printf("\tdivl auxl\n");
            break;
        case INT64:
            printf("\tdivq auxq\n");
            break;
        case FLT32:
            printf("\tdivf auxf\n");
            break;
        case FLT64:
            printf("\tdivdf auxdf\n");
            break;
    }
}
void subtract(int type) {
	if (semantic_error) return;

	printf("\tmov Acc, reg\n");
	printf("\tpop Acc\n");
	printf("\tsub reg, Acc\n");
}
void multiply(int type) {
	if (semantic_error) return;

	printf("\tpop reg\n");
	printf("\tmul reg, Acc\n");
}
/*
void divide(int type) {
	printf("\tmov Acc, reg\n");
	printf("\tpop Acc\n");
	printf("\tdiv reg, Acc\n");
}
*/
/*
void negate(int type) {
	printf("\tnegate Acc\n");
}
*/
void cmp(int relop, int type, char *aux, char *acc) {
	if (semantic_error) return;

    switch(type) {
        case BOOL:
            switch(relop) {
                case '>':
                    printf("\taboveb %sb, %sb\n", aux, acc);
                    break;
                case '<':
                    printf("\tbellowb %sb, %sb\n", aux, acc);
                    break;
                /*
                case "==":
                    break;
                case "!=":
                    break;
                */
            }
            break;

            break;
        case INT32:
            printf("\tcmpl auxl\n");
            break;
        case INT64:
            printf("\tcmpq auxq\n");
            break;
        case FLT32:
            printf("\tcmpf auxf\n");
            break;
        case FLT64:
            printf("\tcmpdf auxdf\n");
            break;
    }
}
void push(int type) {
	if (semantic_error) return;

	printf("\tpush Acc\n");
}
void mov(int type, const char *src, const char *dest) {
	if (semantic_error) return;

	printf("\tmov %s, %s\n", src, dest);
}
void gofalse(int label) {
	if (semantic_error) return;

	printf("\tgofalse .L%d\n", label);
}
void gotolabel(int label) {
	if (semantic_error) return;

	printf("\tgoto .L%d\n", label);
}
void mklabel(int label) {
	if (semantic_error) return;

	printf(".L%d:\n", label);
}
