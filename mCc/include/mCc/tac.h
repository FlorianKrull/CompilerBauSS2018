#ifndef MCC_TAC_H
#define MCC_TAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Reference: Compiler Construction, p. 403*/
enum mCc_tac_op_type {
	MCC_TAC_OP_TYPE_READ,
	MCC_TAC_OP_TYPE_WRITE,
	MCC_TAC_OP_TYPE_LABEL,
	MCC_TAC_OP_TYPE_HALT,
	MCC_TAC_OP_TYPE_IF_FALSE,
	MCC_TAC_OP_TYPE_GOTO,
	MCC_TAC_OP_TYPE_WHILE,
	MCC_TAC_OP_TYPE_RETURN,

	MCC_TAC_OP_TYPE_ADD,
	MCC_TAC_OP_TYPE_SUB,
	MCC_TAC_OP_TYPE_MUL,
	MCC_TAC_OP_TYPE_DIV,

	MCC_TAC_OP_TYPE_GRT,
	MCC_TAC_OP_TYPE_SMT,
	MCC_TAC_OP_TYPE_GRE,
	MCC_TAC_OP_TYPE_SME,
	MCC_TAC_OP_TYPE_EQ,
	MCC_TAC_OP_TYPE_UEQ,
};

enum mCc_tac_address_type {
	MCC_TAC_ADDR_TYPE_EMPTY,
	MCC_TAC_ADDR_TYPE_INT,
	MCC_TAC_ADDR_TYPE_FLOAT,
	MCC_TAC_ADDR_TYPE_BOOL,
	MCC_TAC_ADDR_TYPE_STRING,
	MCC_TAC_ADDR_TYPE_VOID,	// does it neccessary?
};

struct mCc_tac_address {
	enum mCc_tac_address_type type;
	union {
		int i_value;
		float f_value;
		bool b_value;
		char *s_value;
	} content;
};

struct mCc_tac_quad {
	enum mCc_tac_op_type op_type;
	struct mCc_tac_address *addr1, *addr2, *addr3;
};

struct

#ifdef __cplusplus
}
#endif

#endif
