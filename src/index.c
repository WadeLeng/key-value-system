/*============================================================================
# Author: Wade Leng
# E-mail: wade.hit@gmail.com
# Last modified:	2012-02-04 16:59
# Filename:		index.c
# Description: 
============================================================================*/
#include "index.h"
#include "layout.h"
#include "type.h"

#include <stdio.h>

typedef struct IDX_NODE 
{
	IDX_VALUE_INFO			value_node;
	PTR_KW					left_id, right_id;
	HASH					hash_2, hash_3;
}IDX_NODE;

extern	FILE*		log_file;
static	PTR_KW*		ht_table = NULL;  
static	IDX_NODE* 	idx_nodes = NULL;
static	PTR_KW*		free_idx_nodes_stack = NULL;
static	PTR_KW*		free_idx_nodes_top;

static	HASH	hash_func_1(const char* key, int key_size);
static	HASH	hash_func_2(const char* key, int key_size);
static	HASH	hash_func_3(const char* key, int key_size);

static	int		_is_hash_same(HASH hash_x2, HASH hash_x3, HASH hash_y2, HASH hash_y3);
static	int		_get_free_idx_node();
static	void	_put_free_idx_node(int node_id);

int idx_init(const char* image, INIT_TYPE init_type)
{
	int i;
	PTR_KW j;

	ht_table = (PTR_KW*)(image + IMAGE_HT_TABLE);
	idx_nodes = (IDX_NODE*)(image + IMAGE_IDX_NODES);
	free_idx_nodes_stack = (PTR_KW*)(image + IMAGE_FREE_IDX_NODES);
	free_idx_nodes_top = (PTR_KW*) (image + IMAGE_FREE_IDX_NODES_HORIZON);

	if (init_type == INIT_TYPE_CREATE)
	{
		for (i = 0; i < IDX_HT_TABLE_SIZE; i++)
			ht_table[i] = IDX_NODE_NULL;
		for (j = 0; j < IDX_NODES_MAX; j++)
			free_idx_nodes_stack[j] = j;
		*free_idx_nodes_top = IDX_NODES_MAX - 1;
	}
	else if (init_type == INIT_TYPE_LOAD)
	{
		for (i = 0; i < IDX_NODES_MAX; i++)	
			idx_nodes[i].value_node.buf_ptr = BUF_PTR_NULL;
	}

	fprintf(log_file, "INDEX INIT SUCCESS.\n");
	return 0;
}

int idx_insert(const char* key, int key_size, IDX_VALUE_INFO** insert_node_ptr)
{
	int ht_table_id, node_id, new_node_id, cmp;
	HASH cur_hash_2, cur_hash_3;
	PTR_KW*	pre_node_ptr;

	cur_hash_2 = hash_func_2(key, key_size);
	cur_hash_3 = hash_func_3(key, key_size);
	
	ht_table_id = hash_func_1(key, key_size) % IDX_HT_TABLE_SIZE; 
	pre_node_ptr = &ht_table[ht_table_id];
	node_id = ht_table[ht_table_id];

	while (node_id != IDX_NODE_NULL)
	{
		cmp = _is_hash_same(idx_nodes[node_id].hash_2, idx_nodes[node_id].hash_3, cur_hash_2, cur_hash_3);
		if (cmp == 0) {
			return -1;
		} else if (cmp < 0) {
			pre_node_ptr = &(idx_nodes[node_id].left_id);
			node_id = idx_nodes[node_id].left_id;
		} else {
			pre_node_ptr = &(idx_nodes[node_id].right_id);
			node_id = idx_nodes[node_id].right_id;
		}
	}

	new_node_id = _get_free_idx_node();
	idx_nodes[new_node_id].hash_2 = cur_hash_2;
	idx_nodes[new_node_id].hash_3 = cur_hash_3;
	idx_nodes[new_node_id].left_id = idx_nodes[new_node_id].right_id = IDX_NODE_NULL;
	*pre_node_ptr = new_node_id;	
	*insert_node_ptr = (IDX_VALUE_INFO*) &(idx_nodes[new_node_id].value_node);

	return 0;
}

int idx_search(const char* key, int key_size, IDX_VALUE_INFO** search_node)
{
	HASH cur_hash_2, cur_hash_3;
	int ht_table_id, node_id, cmp;

	(*search_node)->buf_ptr = BUF_PTR_NULL;
	(*search_node)->disk_offset = DISK_OFFSET_NULL;
	(*search_node)->value_size = -1;

	cur_hash_2 = hash_func_2(key, key_size);
	cur_hash_3 = hash_func_3(key, key_size);

	ht_table_id = hash_func_1(key, key_size) % IDX_HT_TABLE_SIZE;
	node_id = ht_table[ht_table_id];

	while(node_id != IDX_NODE_NULL)
	{	
		cmp = _is_hash_same(idx_nodes[node_id].hash_2, idx_nodes[node_id].hash_3, cur_hash_2, cur_hash_3);
		if (cmp == 0) {
			(*search_node)->value_size = idx_nodes[node_id].value_node.value_size;		
			(*search_node)->buf_ptr = idx_nodes[node_id].value_node.buf_ptr;
			(*search_node)->disk_offset = idx_nodes[node_id].value_node.disk_offset;
			return 0;
		} else if (cmp < 0) {
			node_id = idx_nodes[node_id].left_id;
		} else {
			node_id = idx_nodes[node_id].right_id;
		}
	}

	return -1;	
}

int idx_delete(const char* key, int key_size, IDX_VALUE_INFO* delete_node)
{
	int ht_table_id, node_id, cmp, nearest_id;
	HASH cur_hash_2, cur_hash_3;
	PTR_KW* pre_node_ptr;
	PTR_KW* nearest_pre_node_ptr;
	
	cur_hash_2 = hash_func_2(key, key_size);
	cur_hash_3 = hash_func_3(key, key_size);
	
	ht_table_id = hash_func_1(key, key_size) % IDX_HT_TABLE_SIZE; 
	pre_node_ptr = &ht_table[ht_table_id];
	node_id = ht_table[ht_table_id];

	while ( 1 )
	{
		if (node_id == IDX_NODE_NULL)
			return -1;

		cmp = _is_hash_same(idx_nodes[node_id].hash_2, idx_nodes[node_id].hash_3, cur_hash_2, cur_hash_3);
		if (cmp == 0) {
			break;
		} else if (cmp < 0) {
			pre_node_ptr = &idx_nodes[node_id].left_id;
			node_id = idx_nodes[node_id].left_id;
		} else {
			pre_node_ptr = &idx_nodes[node_id].right_id;
			node_id = idx_nodes[node_id].right_id;
		}
	}

	delete_node->buf_ptr = idx_nodes[node_id].value_node.buf_ptr;
	delete_node->disk_offset = idx_nodes[node_id].value_node.disk_offset;
	delete_node->value_size = idx_nodes[node_id].value_node.value_size;

	if (idx_nodes[node_id].left_id == IDX_NODE_NULL && idx_nodes[node_id].right_id == IDX_NODE_NULL) {
		*pre_node_ptr = IDX_NODE_NULL;
	} else if (idx_nodes[node_id].left_id != IDX_NODE_NULL && idx_nodes[node_id].right_id != IDX_NODE_NULL) {
		nearest_pre_node_ptr = &idx_nodes[node_id].right_id;
		nearest_id = idx_nodes[node_id].right_id;
		while (idx_nodes[nearest_id].left_id != IDX_NODE_NULL) {
			nearest_pre_node_ptr = &idx_nodes[nearest_id].left_id;
			nearest_id = idx_nodes[nearest_id].left_id;
		}

		*nearest_pre_node_ptr = idx_nodes[nearest_id].right_id;
		idx_nodes[nearest_id].left_id = idx_nodes[node_id].left_id;
		idx_nodes[nearest_id].right_id = idx_nodes[node_id].right_id;
		*pre_node_ptr = nearest_id;
	} else if (idx_nodes[node_id].left_id != IDX_NODE_NULL) {
		*pre_node_ptr = idx_nodes[node_id].left_id;		
	} else {
		*pre_node_ptr = idx_nodes[node_id].right_id;
	}

	_put_free_idx_node(node_id);

	return 0;
}

int idx_exit()
{
	if (log_file)
		fprintf(log_file, "INDEX EXIT.\n");

	return 0;
}

static HASH hash_func_1(const char* key, int key_size)
{
	unsigned int hash = 5381;
	int i =0;
	while (i < key_size) {
		hash += (hash << 5) + *(key+i);
		i++;
	}
	return (hash & 0x7FFFFFFF);
}

static HASH hash_func_2(const char* key, int key_size)
{
	int hash = 0, x = 0, i = 0;
	while (i < key_size) {
		hash = (hash << 4) + *(key+i);
		if ((x = hash & 0xF0000000L) != 0) {
			hash ^= (x >> 24);
			hash &= ~x;
		}
		i++;
	}
	return hash;
}

static HASH hash_func_3(const char* key, int key_size)
{
	int hash = 0, i;
 
	for (i=0; i < key_size; i++) {
		if ((i & 1) == 0) {
			hash ^= ((hash << 7) ^ (*(key+i)) ^ (hash >> 3));
		} else {
			hash ^= (~((hash << 11) ^ (*(key+i)) ^ (hash >> 5)));
		}
	}
	return hash;
}

static int _is_hash_same(HASH hash_x2, HASH hash_x3, HASH hash_y2, HASH hash_y3)
{
	if (hash_y2 == hash_x2)
	{
		if (hash_y3 == hash_x3)
			return 0;
		if (hash_y3 < hash_x3)
			return -1;
		else return 1;
	}
	if (hash_y2 < hash_x2)
		return -1;
	return 1;
}

static int _get_free_idx_node()
{
	if (*free_idx_nodes_top < 0) 
		return -1;
	return free_idx_nodes_stack[(*free_idx_nodes_top)--];
}

static void _put_free_idx_node(int node_id)
{
	free_idx_nodes_stack[++(*free_idx_nodes_top)] = node_id;
}
