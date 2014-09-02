/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef DIAG_DCI_H
#define DIAG_DCI_H

#define MAX_DCI_CLIENTS		10
#define DCI_PKT_RSP_CODE	0x93
#define DCI_DELAYED_RSP_CODE	0x94
#define LOG_CMD_CODE		0x10
#define EVENT_CMD_CODE		0x60
#define DCI_PKT_RSP_TYPE	0
#define DCI_LOG_TYPE		-1
#define DCI_EVENT_TYPE		-2
#define SET_LOG_MASK		1
#define DISABLE_LOG_MASK	0
#define MAX_EVENT_SIZE		512
#define DCI_CLIENT_INDEX_INVALID -1
#define DCI_PKT_REQ_MIN_LEN		5
#define DCI_LOG_CON_MIN_LEN		14
#define DCI_EVENT_CON_MIN_LEN		16

#define DIAG_DATA_TYPE		1
#define DIAG_CNTL_TYPE		2

#define DCI_BUF_PRIMARY		1
#define DCI_BUF_SECONDARY	2
#define DCI_BUF_CMD		3

#ifdef CONFIG_DEBUG_FS
#define DIAG_DCI_DEBUG_CNT	100
#define DIAG_DCI_DEBUG_LEN	100
#endif

/* 16 log code categories, each has:
 * 1 bytes equip id + 1 dirty byte + 512 byte max log mask
 */
#define DCI_LOG_MASK_SIZE		(16*514)
#define DCI_EVENT_MASK_SIZE		512
#define DCI_MASK_STREAM			2
#define DCI_MAX_LOG_CODES		16
#define DCI_MAX_ITEMS_PER_LOG_CODE	512

extern unsigned int dci_max_reg;
extern unsigned int dci_max_clients;
extern unsigned char dci_cumulative_log_mask[DCI_LOG_MASK_SIZE];
extern unsigned char dci_cumulative_event_mask[DCI_EVENT_MASK_SIZE];
extern struct mutex dci_health_mutex;

struct dci_pkt_req_tracking_tbl {
	int pid;
	int uid;
	int tag;
};

struct diag_dci_health_t {
	int dropped_logs;
	int dropped_events;
	int received_logs;
	int received_events;
};

struct diag_dci_buffer_t {
	unsigned char *data;
	unsigned int data_len;
	struct mutex data_mutex;
	uint8_t in_busy;
	uint8_t buf_type;
	int data_source;
	int capacity;
	uint8_t in_list;
	struct list_head buf_track;
};

struct diag_dci_buf_peripheral_t {
	struct diag_dci_buffer_t *buf_curr;
	struct diag_dci_buffer_t *buf_primary;
	struct diag_dci_buffer_t *buf_cmd;
	struct diag_dci_health_t health;
	struct mutex health_mutex;
	struct mutex buf_mutex;
};

struct diag_dci_client_tbl {
	struct task_struct *client;
	uint16_t list;
	int signal_type;
	unsigned char *dci_log_mask;
	unsigned char *dci_event_mask;
	uint8_t real_time;
	struct list_head track;
	struct diag_dci_buf_peripheral_t buffers[NUM_DCI_PROC];
	uint8_t in_service;
	struct list_head list_write_buf;
	struct mutex write_buf_mutex;
};

struct diag_dci_health_stats {
	struct diag_dci_health_t stats;
	int reset_status;
};

struct diag_dci_health_stats_proc {
	struct diag_dci_health_stats *health;
	int proc;
};

/* This is used for querying DCI Log
   or Event Mask */
struct diag_log_event_stats {
	uint16_t code;
	int is_set;
};

enum {
	DIAG_DCI_NO_ERROR = 1001,	/* No error */
	DIAG_DCI_NO_REG,		/* Could not register */
	DIAG_DCI_NO_MEM,		/* Failed memory allocation */
	DIAG_DCI_NOT_SUPPORTED,	/* This particular client is not supported */
	DIAG_DCI_HUGE_PACKET,	/* Request/Response Packet too huge */
	DIAG_DCI_SEND_DATA_FAIL,/* writing to kernel or peripheral fails */
	DIAG_DCI_TABLE_ERR	/* Error dealing with registration tables */
};

#ifdef CONFIG_DEBUG_FS
/* To collect debug information during each smd read */
struct diag_dci_data_info {
	unsigned long iteration;
	int data_size;
	char time_stamp[DIAG_TS_SIZE];
	uint8_t peripheral;
	uint8_t ch_type;
};

extern struct diag_dci_data_info *dci_data_smd;
extern struct mutex dci_stat_mutex;
#endif

int diag_dci_init(void);
void diag_dci_exit(void);
int diag_dci_register_client(uint16_t peripheral_list, int signal);
int diag_dci_deinit_client(void);
void diag_update_smd_dci_work_fn(struct work_struct *);
void diag_dci_notify_client(int peripheral_mask, int data);
void diag_dci_wakeup_clients(void);
void diag_process_apps_dci_read_data(int data_type, void *buf, int recd_bytes);
int diag_process_smd_dci_read_data(struct diag_smd_info *smd_info, void *buf,
								int recd_bytes);
int diag_process_dci_transaction(unsigned char *buf, int len);
int diag_send_dci_pkt(struct diag_master_table entry, unsigned char *buf,
							 int len, int index);
void extract_dci_pkt_rsp(struct diag_smd_info *smd_info, unsigned char *buf,
								int len);
struct diag_dci_client_tbl *diag_dci_get_client_entry(void);
/* DCI Log streaming functions */
void create_dci_log_mask_tbl(unsigned char *tbl_buf);
void update_dci_cumulative_log_mask(int offset, unsigned int byte_index,
						uint8_t byte_mask);
void diag_dci_invalidate_cumulative_log_mask(void);
int diag_send_dci_log_mask(void);
void extract_dci_log(unsigned char *buf, int len, int data_source);
int diag_dci_clear_log_mask(void);
int diag_dci_query_log_mask(uint16_t log_code);
/* DCI event streaming functions */
void update_dci_cumulative_event_mask(int offset, uint8_t byte_mask);
void diag_dci_invalidate_cumulative_event_mask(void);
int diag_send_dci_event_mask(void);
void extract_dci_events(unsigned char *buf, int len, int data_source);
void create_dci_event_mask_tbl(unsigned char *tbl_buf);
int diag_dci_clear_event_mask(void);
int diag_dci_query_event_mask(uint16_t event_id);
void diag_dci_smd_record_info(int read_bytes, uint8_t ch_type,
			      uint8_t peripheral);
uint8_t diag_dci_get_cumulative_real_time(void);
int diag_dci_set_real_time(uint8_t real_time);
int diag_dci_copy_health_stats(struct diag_dci_health_stats *stats, int proc);
/* Functions related to DCI wakeup sources */
void diag_dci_try_activate_wakeup_source(void);
void diag_dci_try_deactivate_wakeup_source(void);
int diag_dci_write_proc(int peripheral, int pkt_type, char *buf, int len);
#endif