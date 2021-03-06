#include <stdio.h>

void R_value(int var_type, const char *name) {
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
void subtract(int type) {
	printf("\tmov Acc, reg\n");
	printf("\tpop Acc\n");
	printf("\tsub reg, Acc\n");
}
void multiply(int type) {
	printf("\tpop reg\n");
	printf("\tmul reg, Acc\n");
}
void divide(int type) {
	printf("\tmov Acc, reg\n");
	printf("\tpop Acc\n");
	printf("\tdiv reg, Acc\n");
}
void negate(int type) {
	printf("\tnegate Acc\n");
}
void push(int type) {
	printf("\tpush Acc\n");
}
void mov(int type, const char *src) {
	printf("\tmov %s, Acc\n", src);
}
