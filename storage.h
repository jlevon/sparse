#ifndef STORAGE_H
#define STORAGE_H

/*
 * The "storage" that underlies an incoming/outgoing pseudo. It's
 * basically the backing store for a pseudo, and may be a real hw
 * register, a stack slot or a static symbol. Or nothing at all,
 * since some pseudos can just be recalculated on the fly.
 */
enum storage_type {
	REG_UDEF,
	REG_REG,
	REG_STACK,
	REG_SYM,
	REG_ARG,
	REG_BAD,
};

enum inout_enum {
	STOR_IN,
	STOR_OUT
};

struct storage;
DECLARE_PTR_LIST(storage_ptr_list, struct storage *);

struct storage {
	enum storage_type type;
	pseudo_t pseudo;
	struct storage_ptr_list *users;
	union {
		int regno;
		int offset;
		struct symbol *sym;
	};
};

DECLARE_PTR_LIST(storage_list, struct storage);

struct storage_hash {
	struct basic_block *bb;
	pseudo_t pseudo;
	enum inout_enum inout;
	struct storage *storage;
};

DECLARE_PTR_LIST(storage_hash_list, struct storage_hash);

extern struct storage_hash_list *gather_storage(struct basic_block *, enum inout_enum);
extern void free_storage(void);
extern const char *show_storage(struct storage *);
extern void set_up_storage(struct entrypoint *);

#endif /* STORAGE_H */