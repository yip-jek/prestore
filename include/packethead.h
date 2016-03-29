#ifndef _PACKET_HEAD_H_
#define _PACKET_HEAD_H_

// Packet head struct
struct PacketHead
{
	int type_id;
	char type_extend[20+1];
	int data_count;
	int file_id;
	char nettype[2+1];
	char excid[4+1];
	char bill_period[6+1];
	char file_type[2+1];
	int channel_id1;
	int channel_id2;
	char cdr_file_name[80+1];
	char roll_type [2+1];
	char proc_date[14+1];
};

#endif	// _PACKET_HEAD_H_
