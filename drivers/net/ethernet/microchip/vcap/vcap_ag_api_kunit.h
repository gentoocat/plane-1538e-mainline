/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
 * Microchip VCAP API interface for kunit testing
 * This is a different interface, to be able to include different VCAPs
 */

/* Use same include guard as the official API to be able to override it */
#ifndef __VCAP_AG_API__
#define __VCAP_AG_API__

enum vcap_type {
	VCAP_TYPE_ES2,
	VCAP_TYPE_IS0,
	VCAP_TYPE_IS2,
	VCAP_TYPE_MAX
};

/* Keyfieldset names with origin information */
enum vcap_keyfield_set {
	VCAP_KFS_NO_VALUE,          /* initial value */
	VCAP_KFS_ARP,               /* sparx5 is2 X6, sparx5 es2 X6 */
	VCAP_KFS_ETAG,              /* sparx5 is0 X2 */
	VCAP_KFS_IP4_OTHER,         /* sparx5 is2 X6, sparx5 es2 X6 */
	VCAP_KFS_IP4_TCP_UDP,       /* sparx5 is2 X6, sparx5 es2 X6 */
	VCAP_KFS_IP4_VID,           /* sparx5 es2 X3 */
	VCAP_KFS_IP6_STD,           /* sparx5 is2 X6 */
	VCAP_KFS_IP6_VID,           /* sparx5 is2 X6, sparx5 es2 X6 */
	VCAP_KFS_IP_7TUPLE,         /* sparx5 is2 X12, sparx5 es2 X12 */
	VCAP_KFS_LL_FULL,           /* sparx5 is0 X6 */
	VCAP_KFS_MAC_ETYPE,         /* sparx5 is2 X6, sparx5 es2 X6 */
	VCAP_KFS_MLL,               /* sparx5 is0 X3 */
	VCAP_KFS_NORMAL,            /* sparx5 is0 X6 */
	VCAP_KFS_NORMAL_5TUPLE_IP4,  /* sparx5 is0 X6 */
	VCAP_KFS_NORMAL_7TUPLE,     /* sparx5 is0 X12 */
	VCAP_KFS_PURE_5TUPLE_IP4,   /* sparx5 is0 X3 */
	VCAP_KFS_TRI_VID,           /* sparx5 is0 X2 */
};

/* List of keyfields with description
 *
 * Keys ending in _IS are booleans derived from frame data
 * Keys ending in _CLS are classified frame data
 *
 * VCAP_KF_8021BR_ECID_BASE: W12, sparx5: is0
 *   Used by 802.1BR Bridge Port Extension in an E-Tag
 * VCAP_KF_8021BR_ECID_EXT: W8, sparx5: is0
 *   Used by 802.1BR Bridge Port Extension in an E-Tag
 * VCAP_KF_8021BR_E_TAGGED: W1, sparx5: is0
 *   Set for frames containing an E-TAG (802.1BR Ethertype 893f)
 * VCAP_KF_8021BR_GRP: W2, sparx5: is0
 *   E-Tag group bits in 802.1BR Bridge Port Extension
 * VCAP_KF_8021BR_IGR_ECID_BASE: W12, sparx5: is0
 *   Used by 802.1BR Bridge Port Extension in an E-Tag
 * VCAP_KF_8021BR_IGR_ECID_EXT: W8, sparx5: is0
 *   Used by 802.1BR Bridge Port Extension in an E-Tag
 * VCAP_KF_8021Q_DEI0: W1, sparx5: is0
 *   First DEI in multiple vlan tags (outer tag or default port tag)
 * VCAP_KF_8021Q_DEI1: W1, sparx5: is0
 *   Second DEI in multiple vlan tags (inner tag)
 * VCAP_KF_8021Q_DEI2: W1, sparx5: is0
 *   Third DEI in multiple vlan tags (not always available)
 * VCAP_KF_8021Q_DEI_CLS: W1, sparx5: is2/es2
 *   Classified DEI
 * VCAP_KF_8021Q_PCP0: W3, sparx5: is0
 *   First PCP in multiple vlan tags (outer tag or default port tag)
 * VCAP_KF_8021Q_PCP1: W3, sparx5: is0
 *   Second PCP in multiple vlan tags (inner tag)
 * VCAP_KF_8021Q_PCP2: W3, sparx5: is0
 *   Third PCP in multiple vlan tags (not always available)
 * VCAP_KF_8021Q_PCP_CLS: W3, sparx5: is2/es2
 *   Classified PCP
 * VCAP_KF_8021Q_TPID0: W3, sparx5: is0
 *   First TPIC in multiple vlan tags (outer tag or default port tag)
 * VCAP_KF_8021Q_TPID1: W3, sparx5: is0
 *   Second TPID in multiple vlan tags (inner tag)
 * VCAP_KF_8021Q_TPID2: W3, sparx5: is0
 *   Third TPID in multiple vlan tags (not always available)
 * VCAP_KF_8021Q_VID0: W12, sparx5: is0
 *   First VID in multiple vlan tags (outer tag or default port tag)
 * VCAP_KF_8021Q_VID1: W12, sparx5: is0
 *   Second VID in multiple vlan tags (inner tag)
 * VCAP_KF_8021Q_VID2: W12, sparx5: is0
 *   Third VID in multiple vlan tags (not always available)
 * VCAP_KF_8021Q_VID_CLS: W13, sparx5: is2/es2
 *   Classified VID
 * VCAP_KF_8021Q_VLAN_TAGGED_IS: W1, sparx5: is2/es2
 *   Sparx5: Set if frame was received with a VLAN tag, LAN966x: Set if frame has
 *   one or more Q-tags. Independent of port VLAN awareness
 * VCAP_KF_8021Q_VLAN_TAGS: W3, sparx5: is0
 *   Number of VLAN tags in frame: 0: Untagged, 1: Single tagged, 3: Double
 *   tagged, 7: Triple tagged
 * VCAP_KF_ACL_GRP_ID: W8, sparx5: es2
 *   Used in interface map table
 * VCAP_KF_ARP_ADDR_SPACE_OK_IS: W1, sparx5: is2/es2
 *   Set if hardware address is Ethernet
 * VCAP_KF_ARP_LEN_OK_IS: W1, sparx5: is2/es2
 *   Set if hardware address length = 6 (Ethernet) and IP address length = 4 (IP).
 * VCAP_KF_ARP_OPCODE: W2, sparx5: is2/es2
 *   ARP opcode
 * VCAP_KF_ARP_OPCODE_UNKNOWN_IS: W1, sparx5: is2/es2
 *   Set if not one of the codes defined in VCAP_KF_ARP_OPCODE
 * VCAP_KF_ARP_PROTO_SPACE_OK_IS: W1, sparx5: is2/es2
 *   Set if protocol address space is 0x0800
 * VCAP_KF_ARP_SENDER_MATCH_IS: W1, sparx5: is2/es2
 *   Sender Hardware Address = SMAC (ARP)
 * VCAP_KF_ARP_TGT_MATCH_IS: W1, sparx5: is2/es2
 *   Target Hardware Address = SMAC (RARP)
 * VCAP_KF_COSID_CLS: W3, sparx5: es2
 *   Class of service
 * VCAP_KF_DST_ENTRY: W1, sparx5: is0
 *   Selects whether the frame’s destination or source information is used for
 *   fields L2_SMAC and L3_IP4_SIP
 * VCAP_KF_ES0_ISDX_KEY_ENA: W1, sparx5: es2
 *   The value taken from the IFH .FWD.ES0_ISDX_KEY_ENA
 * VCAP_KF_ETYPE: W16, sparx5: is0/is2/es2
 *   Ethernet type
 * VCAP_KF_ETYPE_LEN_IS: W1, sparx5: is0/is2/es2
 *   Set if frame has EtherType >= 0x600
 * VCAP_KF_ETYPE_MPLS: W2, sparx5: is0
 *   Type of MPLS Ethertype (or not)
 * VCAP_KF_IF_EGR_PORT_MASK: W32, sparx5: es2
 *   Egress port mask, one bit per port
 * VCAP_KF_IF_EGR_PORT_MASK_RNG: W3, sparx5: es2
 *   Select which 32 port group is available in IF_EGR_PORT (or virtual ports or
 *   CPU queue)
 * VCAP_KF_IF_IGR_PORT: sparx5 is0 W7, sparx5 es2 W9
 *   Sparx5: Logical ingress port number retrieved from
 *   ANA_CL::PORT_ID_CFG.LPORT_NUM or ERLEG, LAN966x: ingress port nunmber
 * VCAP_KF_IF_IGR_PORT_MASK: sparx5 is0 W65, sparx5 is2 W32, sparx5 is2 W65
 *   Ingress port mask, one bit per port/erleg
 * VCAP_KF_IF_IGR_PORT_MASK_L3: W1, sparx5: is2
 *   If set, IF_IGR_PORT_MASK, IF_IGR_PORT_MASK_RNG, and IF_IGR_PORT_MASK_SEL are
 *   used to specify L3 interfaces
 * VCAP_KF_IF_IGR_PORT_MASK_RNG: W4, sparx5: is2
 *   Range selector for IF_IGR_PORT_MASK.  Specifies which group of 32 ports are
 *   available in IF_IGR_PORT_MASK
 * VCAP_KF_IF_IGR_PORT_MASK_SEL: W2, sparx5: is0/is2
 *   Mode selector for IF_IGR_PORT_MASK, applicable when IF_IGR_PORT_MASK_L3 == 0.
 *   Mapping: 0: DEFAULT 1: LOOPBACK 2: MASQUERADE 3: CPU_VD
 * VCAP_KF_IF_IGR_PORT_SEL: W1, sparx5: es2
 *   Selector for IF_IGR_PORT: physical port number or ERLEG
 * VCAP_KF_IP4_IS: W1, sparx5: is0/is2/es2
 *   Set if frame has EtherType = 0x800 and IP version = 4
 * VCAP_KF_IP_MC_IS: W1, sparx5: is0
 *   Set if frame is IPv4 frame and frame’s destination MAC address is an IPv4
 *   multicast address (0x01005E0 /25). Set if frame is IPv6 frame and frame’s
 *   destination MAC address is an IPv6 multicast address (0x3333/16).
 * VCAP_KF_IP_PAYLOAD_5TUPLE: W32, sparx5: is0
 *   Payload bytes after IP header
 * VCAP_KF_IP_SNAP_IS: W1, sparx5: is0
 *   Set if frame is IPv4, IPv6, or SNAP frame
 * VCAP_KF_ISDX_CLS: W12, sparx5: is2/es2
 *   Classified ISDX
 * VCAP_KF_ISDX_GT0_IS: W1, sparx5: is2/es2
 *   Set if classified ISDX > 0
 * VCAP_KF_L2_BC_IS: W1, sparx5: is0/is2/es2
 *   Set if frame’s destination MAC address is the broadcast address
 *   (FF-FF-FF-FF-FF-FF).
 * VCAP_KF_L2_DMAC: W48, sparx5: is0/is2/es2
 *   Destination MAC address
 * VCAP_KF_L2_FWD_IS: W1, sparx5: is2
 *   Set if the frame is allowed to be forwarded to front ports
 * VCAP_KF_L2_MC_IS: W1, sparx5: is0/is2/es2
 *   Set if frame’s destination MAC address is a multicast address (bit 40 = 1).
 * VCAP_KF_L2_PAYLOAD_ETYPE: W64, sparx5: is2/es2
 *   Byte 0-7 of L2 payload after Type/Len field and overloading for OAM
 * VCAP_KF_L2_SMAC: W48, sparx5: is0/is2/es2
 *   Source MAC address
 * VCAP_KF_L3_DIP_EQ_SIP_IS: W1, sparx5: is2/es2
 *   Set if Src IP matches Dst IP address
 * VCAP_KF_L3_DMAC_DIP_MATCH: W1, sparx5: is2
 *   Match found in DIP security lookup in ANA_L3
 * VCAP_KF_L3_DPL_CLS: W1, sparx5: es2
 *   The frames drop precedence level
 * VCAP_KF_L3_DSCP: W6, sparx5: is0
 *   Frame’s DSCP value
 * VCAP_KF_L3_DST_IS: W1, sparx5: is2
 *   Set if lookup is done for egress router leg
 * VCAP_KF_L3_FRAGMENT_TYPE: W2, sparx5: is0/is2/es2
 *   L3 Fragmentation type (none, initial, suspicious, valid follow up)
 * VCAP_KF_L3_FRAG_INVLD_L4_LEN: W1, sparx5: is0/is2
 *   Set if frame's L4 length is less than ANA_CL:COMMON:CLM_FRAGMENT_CFG.L4_MIN_L
 *   EN
 * VCAP_KF_L3_IP4_DIP: W32, sparx5: is0/is2/es2
 *   Destination IPv4 Address
 * VCAP_KF_L3_IP4_SIP: W32, sparx5: is0/is2/es2
 *   Source IPv4 Address
 * VCAP_KF_L3_IP6_DIP: W128, sparx5: is0/is2/es2
 *   Sparx5: Full IPv6 DIP, LAN966x: Either Full IPv6 DIP or a subset depending on
 *   frame type
 * VCAP_KF_L3_IP6_SIP: W128, sparx5: is0/is2/es2
 *   Sparx5: Full IPv6 SIP, LAN966x: Either Full IPv6 SIP or a subset depending on
 *   frame type
 * VCAP_KF_L3_IP_PROTO: W8, sparx5: is0/is2/es2
 *   IPv4 frames: IP protocol. IPv6 frames: Next header, same as for IPV4
 * VCAP_KF_L3_OPTIONS_IS: W1, sparx5: is0/is2/es2
 *   Set if IPv4 frame contains options (IP len > 5)
 * VCAP_KF_L3_PAYLOAD: sparx5 is2 W96, sparx5 is2 W40, sparx5 es2 W96
 *   Sparx5: Payload bytes after IP header. IPv4: IPv4 options are not parsed so
 *   payload is always taken 20 bytes after the start of the IPv4 header, LAN966x:
 *   Bytes 0-6 after IP header
 * VCAP_KF_L3_RT_IS: W1, sparx5: is2/es2
 *   Set if frame has hit a router leg
 * VCAP_KF_L3_SMAC_SIP_MATCH: W1, sparx5: is2
 *   Match found in SIP security lookup in ANA_L3
 * VCAP_KF_L3_TOS: W8, sparx5: is2/es2
 *   Sparx5: Frame's IPv4/IPv6 DSCP and ECN fields, LAN966x: IP TOS field
 * VCAP_KF_L3_TTL_GT0: W1, sparx5: is2/es2
 *   Set if IPv4 TTL / IPv6 hop limit is greater than 0
 * VCAP_KF_L4_ACK: W1, sparx5: is2/es2
 *   Sparx5 and LAN966x: TCP flag ACK, LAN966x only: PTP over UDP: flagField bit 2
 *   (unicastFlag)
 * VCAP_KF_L4_DPORT: W16, sparx5: is2/es2
 *   Sparx5: TCP/UDP destination port. Overloading for IP_7TUPLE: Non-TCP/UDP IP
 *   frames: L4_DPORT = L3_IP_PROTO, LAN966x: TCP/UDP destination port
 * VCAP_KF_L4_FIN: W1, sparx5: is2/es2
 *   TCP flag FIN, LAN966x: TCP flag FIN, and for PTP over UDP: messageType bit 1
 * VCAP_KF_L4_PAYLOAD: W64, sparx5: is2/es2
 *   Payload bytes after TCP/UDP header Overloading for IP_7TUPLE: Non TCP/UDP
 *   frames: Payload bytes 0–7 after IP header. IPv4 options are not parsed so
 *   payload is always taken 20 bytes after the start of the IPv4 header for non
 *   TCP/UDP IPv4 frames
 * VCAP_KF_L4_PSH: W1, sparx5: is2/es2
 *   Sparx5: TCP flag PSH, LAN966x: TCP: TCP flag PSH. PTP over UDP: flagField bit
 *   1 (twoStepFlag)
 * VCAP_KF_L4_RNG: sparx5 is0 W8, sparx5 is2 W16, sparx5 es2 W16
 *   Range checker bitmask (one for each range checker). Input into range checkers
 *   is taken from classified results (VID, DSCP) and frame (SPORT, DPORT, ETYPE,
 *   outer VID, inner VID)
 * VCAP_KF_L4_RST: W1, sparx5: is2/es2
 *   Sparx5: TCP flag RST , LAN966x: TCP: TCP flag RST. PTP over UDP: messageType
 *   bit 3
 * VCAP_KF_L4_SEQUENCE_EQ0_IS: W1, sparx5: is2/es2
 *   Set if TCP sequence number is 0, LAN966x: Overlayed with PTP over UDP:
 *   messageType bit 0
 * VCAP_KF_L4_SPORT: W16, sparx5: is0/is2/es2
 *   TCP/UDP source port
 * VCAP_KF_L4_SPORT_EQ_DPORT_IS: W1, sparx5: is2/es2
 *   Set if UDP or TCP source port equals UDP or TCP destination port
 * VCAP_KF_L4_SYN: W1, sparx5: is2/es2
 *   Sparx5: TCP flag SYN, LAN966x: TCP: TCP flag SYN. PTP over UDP: messageType
 *   bit 2
 * VCAP_KF_L4_URG: W1, sparx5: is2/es2
 *   Sparx5: TCP flag URG, LAN966x: TCP: TCP flag URG. PTP over UDP: flagField bit
 *   7 (reserved)
 * VCAP_KF_LOOKUP_FIRST_IS: W1, sparx5: is0/is2/es2
 *   Selects between entries relevant for first and second lookup. Set for first
 *   lookup, cleared for second lookup.
 * VCAP_KF_LOOKUP_GEN_IDX: W12, sparx5: is0
 *   Generic index - for chaining CLM instances
 * VCAP_KF_LOOKUP_GEN_IDX_SEL: W2, sparx5: is0
 *   Select the mode of the Generic Index
 * VCAP_KF_LOOKUP_PAG: W8, sparx5: is2
 *   Classified Policy Association Group: chains rules from IS1/CLM to IS2
 * VCAP_KF_OAM_CCM_CNTS_EQ0: W1, sparx5: is2/es2
 *   Dual-ended loss measurement counters in CCM frames are all zero
 * VCAP_KF_OAM_MEL_FLAGS: W7, sparx5: is0
 *   Encoding of MD level/MEG level (MEL)
 * VCAP_KF_OAM_Y1731_IS: W1, sparx5: is0/is2/es2
 *   Set if frame’s EtherType = 0x8902
 * VCAP_KF_PROT_ACTIVE: W1, sparx5: es2
 *   Protection is active
 * VCAP_KF_TCP_IS: W1, sparx5: is0/is2/es2
 *   Set if frame is IPv4 TCP frame (IP protocol = 6) or IPv6 TCP frames (Next
 *   header = 6)
 * VCAP_KF_TCP_UDP_IS: W1, sparx5: is0/is2/es2
 *   Set if frame is IPv4/IPv6 TCP or UDP frame (IP protocol/next header equals 6
 *   or 17)
 * VCAP_KF_TYPE: sparx5 is0 W2, sparx5 is0 W1, sparx5 is2 W4, sparx5 is2 W2,
 *   sparx5 es2 W3
 *   Keyset type id - set by the API
 */

/* Keyfield names */
enum vcap_key_field {
	VCAP_KF_NO_VALUE,  /* initial value */
	VCAP_KF_8021BR_ECID_BASE,
	VCAP_KF_8021BR_ECID_EXT,
	VCAP_KF_8021BR_E_TAGGED,
	VCAP_KF_8021BR_GRP,
	VCAP_KF_8021BR_IGR_ECID_BASE,
	VCAP_KF_8021BR_IGR_ECID_EXT,
	VCAP_KF_8021Q_DEI0,
	VCAP_KF_8021Q_DEI1,
	VCAP_KF_8021Q_DEI2,
	VCAP_KF_8021Q_DEI_CLS,
	VCAP_KF_8021Q_PCP0,
	VCAP_KF_8021Q_PCP1,
	VCAP_KF_8021Q_PCP2,
	VCAP_KF_8021Q_PCP_CLS,
	VCAP_KF_8021Q_TPID0,
	VCAP_KF_8021Q_TPID1,
	VCAP_KF_8021Q_TPID2,
	VCAP_KF_8021Q_VID0,
	VCAP_KF_8021Q_VID1,
	VCAP_KF_8021Q_VID2,
	VCAP_KF_8021Q_VID_CLS,
	VCAP_KF_8021Q_VLAN_TAGGED_IS,
	VCAP_KF_8021Q_VLAN_TAGS,
	VCAP_KF_ACL_GRP_ID,
	VCAP_KF_ARP_ADDR_SPACE_OK_IS,
	VCAP_KF_ARP_LEN_OK_IS,
	VCAP_KF_ARP_OPCODE,
	VCAP_KF_ARP_OPCODE_UNKNOWN_IS,
	VCAP_KF_ARP_PROTO_SPACE_OK_IS,
	VCAP_KF_ARP_SENDER_MATCH_IS,
	VCAP_KF_ARP_TGT_MATCH_IS,
	VCAP_KF_COSID_CLS,
	VCAP_KF_DST_ENTRY,
	VCAP_KF_ES0_ISDX_KEY_ENA,
	VCAP_KF_ETYPE,
	VCAP_KF_ETYPE_LEN_IS,
	VCAP_KF_ETYPE_MPLS,
	VCAP_KF_IF_EGR_PORT_MASK,
	VCAP_KF_IF_EGR_PORT_MASK_RNG,
	VCAP_KF_IF_IGR_PORT,
	VCAP_KF_IF_IGR_PORT_MASK,
	VCAP_KF_IF_IGR_PORT_MASK_L3,
	VCAP_KF_IF_IGR_PORT_MASK_RNG,
	VCAP_KF_IF_IGR_PORT_MASK_SEL,
	VCAP_KF_IF_IGR_PORT_SEL,
	VCAP_KF_IP4_IS,
	VCAP_KF_IP_MC_IS,
	VCAP_KF_IP_PAYLOAD_5TUPLE,
	VCAP_KF_IP_SNAP_IS,
	VCAP_KF_ISDX_CLS,
	VCAP_KF_ISDX_GT0_IS,
	VCAP_KF_L2_BC_IS,
	VCAP_KF_L2_DMAC,
	VCAP_KF_L2_FWD_IS,
	VCAP_KF_L2_MC_IS,
	VCAP_KF_L2_PAYLOAD_ETYPE,
	VCAP_KF_L2_SMAC,
	VCAP_KF_L3_DIP_EQ_SIP_IS,
	VCAP_KF_L3_DMAC_DIP_MATCH,
	VCAP_KF_L3_DPL_CLS,
	VCAP_KF_L3_DSCP,
	VCAP_KF_L3_DST_IS,
	VCAP_KF_L3_FRAGMENT_TYPE,
	VCAP_KF_L3_FRAG_INVLD_L4_LEN,
	VCAP_KF_L3_IP4_DIP,
	VCAP_KF_L3_IP4_SIP,
	VCAP_KF_L3_IP6_DIP,
	VCAP_KF_L3_IP6_SIP,
	VCAP_KF_L3_IP_PROTO,
	VCAP_KF_L3_OPTIONS_IS,
	VCAP_KF_L3_PAYLOAD,
	VCAP_KF_L3_RT_IS,
	VCAP_KF_L3_SMAC_SIP_MATCH,
	VCAP_KF_L3_TOS,
	VCAP_KF_L3_TTL_GT0,
	VCAP_KF_L4_ACK,
	VCAP_KF_L4_DPORT,
	VCAP_KF_L4_FIN,
	VCAP_KF_L4_PAYLOAD,
	VCAP_KF_L4_PSH,
	VCAP_KF_L4_RNG,
	VCAP_KF_L4_RST,
	VCAP_KF_L4_SEQUENCE_EQ0_IS,
	VCAP_KF_L4_SPORT,
	VCAP_KF_L4_SPORT_EQ_DPORT_IS,
	VCAP_KF_L4_SYN,
	VCAP_KF_L4_URG,
	VCAP_KF_LOOKUP_FIRST_IS,
	VCAP_KF_LOOKUP_GEN_IDX,
	VCAP_KF_LOOKUP_GEN_IDX_SEL,
	VCAP_KF_LOOKUP_PAG,
	VCAP_KF_MIRROR_ENA,
	VCAP_KF_OAM_CCM_CNTS_EQ0,
	VCAP_KF_OAM_MEL_FLAGS,
	VCAP_KF_OAM_Y1731_IS,
	VCAP_KF_PROT_ACTIVE,
	VCAP_KF_TCP_IS,
	VCAP_KF_TCP_UDP_IS,
	VCAP_KF_TYPE,
};

/* Actionset names with origin information */
enum vcap_actionfield_set {
	VCAP_AFS_NO_VALUE,          /* initial value */
	VCAP_AFS_BASE_TYPE,         /* sparx5 is2 X3, sparx5 es2 X3 */
	VCAP_AFS_CLASSIFICATION,    /* sparx5 is0 X2 */
	VCAP_AFS_CLASS_REDUCED,     /* sparx5 is0 X1 */
	VCAP_AFS_FULL,              /* sparx5 is0 X3 */
	VCAP_AFS_MLBS,              /* sparx5 is0 X2 */
	VCAP_AFS_MLBS_REDUCED,      /* sparx5 is0 X1 */
};

/* List of actionfields with description
 *
 * VCAP_AF_CLS_VID_SEL: W3, sparx5: is0
 *   Controls the classified VID: 0: VID_NONE: No action. 1: VID_ADD: New VID =
 *   old VID + VID_VAL. 2: VID_REPLACE: New VID = VID_VAL. 3: VID_FIRST_TAG: New
 *   VID = VID from frame's first tag (outer tag) if available, otherwise VID_VAL.
 *   4: VID_SECOND_TAG: New VID = VID from frame's second tag (middle tag) if
 *   available, otherwise VID_VAL. 5: VID_THIRD_TAG: New VID = VID from frame's
 *   third tag (inner tag) if available, otherwise VID_VAL.
 * VCAP_AF_CNT_ID: sparx5 is2 W12, sparx5 es2 W11
 *   Counter ID, used per lookup to index the 4K frame counters (ANA_ACL:CNT_TBL).
 *   Multiple VCAP IS2 entries can use the same counter.
 * VCAP_AF_COPY_PORT_NUM: W7, sparx5: es2
 *   QSYS port number when FWD_MODE is redirect or copy
 * VCAP_AF_COPY_QUEUE_NUM: W16, sparx5: es2
 *   QSYS queue number when FWD_MODE is redirect or copy
 * VCAP_AF_CPU_COPY_ENA: W1, sparx5: is2/es2
 *   Setting this bit to 1 causes all frames that hit this action to be copied to
 *   the CPU extraction queue specified in CPU_QUEUE_NUM.
 * VCAP_AF_CPU_QUEUE_NUM: W3, sparx5: is2/es2
 *   CPU queue number. Used when CPU_COPY_ENA is set.
 * VCAP_AF_DEI_ENA: W1, sparx5: is0
 *   If set, use DEI_VAL as classified DEI value. Otherwise, DEI from basic
 *   classification is used
 * VCAP_AF_DEI_VAL: W1, sparx5: is0
 *   See DEI_ENA
 * VCAP_AF_DP_ENA: W1, sparx5: is0
 *   If set, use DP_VAL as classified drop precedence level. Otherwise, drop
 *   precedence level from basic classification is used.
 * VCAP_AF_DP_VAL: W2, sparx5: is0
 *   See DP_ENA.
 * VCAP_AF_DSCP_ENA: W1, sparx5: is0
 *   If set, use DSCP_VAL as classified DSCP value. Otherwise, DSCP value from
 *   basic classification is used.
 * VCAP_AF_DSCP_VAL: W6, sparx5: is0
 *   See DSCP_ENA.
 * VCAP_AF_ES2_REW_CMD: W3, sparx5: es2
 *   Command forwarded to REW: 0: No action. 1: SWAP MAC addresses. 2: Do L2CP
 *   DMAC translation when entering or leaving a tunnel.
 * VCAP_AF_FWD_MODE: W2, sparx5: es2
 *   Forward selector: 0: Forward. 1: Discard. 2: Redirect. 3: Copy.
 * VCAP_AF_HIT_ME_ONCE: W1, sparx5: is2/es2
 *   Setting this bit to 1 causes the first frame that hits this action where the
 *   HIT_CNT counter is zero to be copied to the CPU extraction queue specified in
 *   CPU_QUEUE_NUM. The HIT_CNT counter is then incremented and any frames that
 *   hit this action later are not copied to the CPU. To re-enable the HIT_ME_ONCE
 *   functionality, the HIT_CNT counter must be cleared.
 * VCAP_AF_IGNORE_PIPELINE_CTRL: W1, sparx5: is2/es2
 *   Ignore ingress pipeline control. This enforces the use of the VCAP IS2 action
 *   even when the pipeline control has terminated the frame before VCAP IS2.
 * VCAP_AF_INTR_ENA: W1, sparx5: is2/es2
 *   If set, an interrupt is triggered when this rule is hit
 * VCAP_AF_ISDX_ADD_REPLACE_SEL: W1, sparx5: is0
 *   Controls the classified ISDX. 0: New ISDX = old ISDX + ISDX_VAL. 1: New ISDX
 *   = ISDX_VAL.
 * VCAP_AF_ISDX_VAL: W12, sparx5: is0
 *   See isdx_add_replace_sel
 * VCAP_AF_LRN_DIS: W1, sparx5: is2
 *   Setting this bit to 1 disables learning of frames hitting this action.
 * VCAP_AF_MAP_IDX: W9, sparx5: is0
 *   Index for QoS mapping table lookup
 * VCAP_AF_MAP_KEY: W3, sparx5: is0
 *   Key type for QoS mapping table lookup. 0: DEI0, PCP0 (outer tag). 1: DEI1,
 *   PCP1 (middle tag). 2: DEI2, PCP2 (inner tag). 3: MPLS TC. 4: PCP0 (outer
 *   tag). 5: E-DEI, E-PCP (E-TAG). 6: DSCP if available, otherwise none. 7: DSCP
 *   if available, otherwise DEI0, PCP0 (outer tag) if available using MAP_IDX+8,
 *   otherwise none
 * VCAP_AF_MAP_LOOKUP_SEL: W2, sparx5: is0
 *   Selects which of the two QoS Mapping Table lookups that MAP_KEY and MAP_IDX
 *   are applied to. 0: No changes to the QoS Mapping Table lookup. 1: Update key
 *   type and index for QoS Mapping Table lookup #0. 2: Update key type and index
 *   for QoS Mapping Table lookup #1. 3: Reserved.
 * VCAP_AF_MASK_MODE: W3, sparx5: is0/is2
 *   Controls the PORT_MASK use. Sparx5: 0: OR_DSTMASK, 1: AND_VLANMASK, 2:
 *   REPLACE_PGID, 3: REPLACE_ALL, 4: REDIR_PGID, 5: OR_PGID_MASK, 6: VSTAX, 7:
 *   Not applicable. LAN966X: 0: No action, 1: Permit/deny (AND), 2: Policy
 *   forwarding (DMAC lookup), 3: Redirect. The CPU port is untouched by
 *   MASK_MODE.
 * VCAP_AF_MATCH_ID: W16, sparx5: is0/is2
 *   Logical ID for the entry. The MATCH_ID is extracted together with the frame
 *   if the frame is forwarded to the CPU (CPU_COPY_ENA). The result is placed in
 *   IFH.CL_RSLT.
 * VCAP_AF_MATCH_ID_MASK: W16, sparx5: is0/is2
 *   Mask used by MATCH_ID.
 * VCAP_AF_MIRROR_PROBE: W2, sparx5: is2
 *   Mirroring performed according to configuration of a mirror probe. 0: No
 *   mirroring. 1: Mirror probe 0. 2: Mirror probe 1. 3: Mirror probe 2
 * VCAP_AF_MIRROR_PROBE_ID: W2, sparx5: es2
 *   Signals a mirror probe to be placed in the IFH. Only possible when FWD_MODE
 *   is copy. 0: No mirroring. 1–3: Use mirror probe 0-2.
 * VCAP_AF_NXT_IDX: W12, sparx5: is0
 *   Index used as part of key (field G_IDX) in the next lookup.
 * VCAP_AF_NXT_IDX_CTRL: W3, sparx5: is0
 *   Controls the generation of the G_IDX used in the VCAP CLM next lookup
 * VCAP_AF_PAG_OVERRIDE_MASK: W8, sparx5: is0
 *   Bits set in this mask will override PAG_VAL from port profile.  New PAG =
 *   (PAG (input) AND ~PAG_OVERRIDE_MASK) OR (PAG_VAL AND PAG_OVERRIDE_MASK)
 * VCAP_AF_PAG_VAL: W8, sparx5: is0
 *   See PAG_OVERRIDE_MASK.
 * VCAP_AF_PCP_ENA: W1, sparx5: is0
 *   If set, use PCP_VAL as classified PCP value. Otherwise, PCP from basic
 *   classification is used.
 * VCAP_AF_PCP_VAL: W3, sparx5: is0
 *   See PCP_ENA.
 * VCAP_AF_PIPELINE_FORCE_ENA: sparx5 is0 W2, sparx5 is2 W1
 *   If set, use PIPELINE_PT unconditionally and set PIPELINE_ACT = NONE if
 *   PIPELINE_PT == NONE. Overrules previous settings of pipeline point.
 * VCAP_AF_PIPELINE_PT: W5, sparx5: is0/is2
 *   Pipeline point used if PIPELINE_FORCE_ENA is set
 * VCAP_AF_POLICE_ENA: W1, sparx5: is2/es2
 *   Setting this bit to 1 causes frames that hit this action to be policed by the
 *   ACL policer specified in POLICE_IDX. Only applies to the first lookup.
 * VCAP_AF_POLICE_IDX: W6, sparx5: is2/es2
 *   Selects VCAP policer used when policing frames (POLICE_ENA)
 * VCAP_AF_POLICE_REMARK: W1, sparx5: es2
 *   If set, frames exceeding policer rates are marked as yellow but not
 *   discarded.
 * VCAP_AF_PORT_MASK: sparx5 is0 W65, sparx5 is2 W68
 *   Port mask applied to the forwarding decision based on MASK_MODE.
 * VCAP_AF_QOS_ENA: W1, sparx5: is0
 *   If set, use QOS_VAL as classified QoS class. Otherwise, QoS class from basic
 *   classification is used.
 * VCAP_AF_QOS_VAL: W3, sparx5: is0
 *   See QOS_ENA.
 * VCAP_AF_RT_DIS: W1, sparx5: is2
 *   If set, routing is disallowed. Only applies when IS_INNER_ACL is 0. See also
 *   IGR_ACL_ENA, EGR_ACL_ENA, and RLEG_STAT_IDX.
 * VCAP_AF_TYPE: W1, sparx5: is0
 *   Actionset type id - Set by the API
 * VCAP_AF_VID_VAL: W13, sparx5: is0
 *   New VID Value
 */

/* Actionfield names */
enum vcap_action_field {
	VCAP_AF_NO_VALUE,  /* initial value */
	VCAP_AF_ACL_MAC,
	VCAP_AF_ACL_RT_MODE,
	VCAP_AF_CLS_VID_SEL,
	VCAP_AF_CNT_ID,
	VCAP_AF_COPY_PORT_NUM,
	VCAP_AF_COPY_QUEUE_NUM,
	VCAP_AF_COSID_ENA,
	VCAP_AF_COSID_VAL,
	VCAP_AF_CPU_COPY_ENA,
	VCAP_AF_CPU_DIS,
	VCAP_AF_CPU_ENA,
	VCAP_AF_CPU_Q,
	VCAP_AF_CPU_QUEUE_NUM,
	VCAP_AF_CUSTOM_ACE_ENA,
	VCAP_AF_CUSTOM_ACE_OFFSET,
	VCAP_AF_DEI_ENA,
	VCAP_AF_DEI_VAL,
	VCAP_AF_DLB_OFFSET,
	VCAP_AF_DMAC_OFFSET_ENA,
	VCAP_AF_DP_ENA,
	VCAP_AF_DP_VAL,
	VCAP_AF_DSCP_ENA,
	VCAP_AF_DSCP_VAL,
	VCAP_AF_EGR_ACL_ENA,
	VCAP_AF_ES2_REW_CMD,
	VCAP_AF_FWD_DIS,
	VCAP_AF_FWD_MODE,
	VCAP_AF_FWD_TYPE,
	VCAP_AF_GVID_ADD_REPLACE_SEL,
	VCAP_AF_HIT_ME_ONCE,
	VCAP_AF_IGNORE_PIPELINE_CTRL,
	VCAP_AF_IGR_ACL_ENA,
	VCAP_AF_INJ_MASQ_ENA,
	VCAP_AF_INJ_MASQ_LPORT,
	VCAP_AF_INJ_MASQ_PORT,
	VCAP_AF_INTR_ENA,
	VCAP_AF_ISDX_ADD_REPLACE_SEL,
	VCAP_AF_ISDX_VAL,
	VCAP_AF_IS_INNER_ACL,
	VCAP_AF_L3_MAC_UPDATE_DIS,
	VCAP_AF_LOG_MSG_INTERVAL,
	VCAP_AF_LPM_AFFIX_ENA,
	VCAP_AF_LPM_AFFIX_VAL,
	VCAP_AF_LPORT_ENA,
	VCAP_AF_LRN_DIS,
	VCAP_AF_MAP_IDX,
	VCAP_AF_MAP_KEY,
	VCAP_AF_MAP_LOOKUP_SEL,
	VCAP_AF_MASK_MODE,
	VCAP_AF_MATCH_ID,
	VCAP_AF_MATCH_ID_MASK,
	VCAP_AF_MIP_SEL,
	VCAP_AF_MIRROR_PROBE,
	VCAP_AF_MIRROR_PROBE_ID,
	VCAP_AF_MPLS_IP_CTRL_ENA,
	VCAP_AF_MPLS_MEP_ENA,
	VCAP_AF_MPLS_MIP_ENA,
	VCAP_AF_MPLS_OAM_FLAVOR,
	VCAP_AF_MPLS_OAM_TYPE,
	VCAP_AF_NUM_VLD_LABELS,
	VCAP_AF_NXT_IDX,
	VCAP_AF_NXT_IDX_CTRL,
	VCAP_AF_NXT_KEY_TYPE,
	VCAP_AF_NXT_NORMALIZE,
	VCAP_AF_NXT_NORM_W16_OFFSET,
	VCAP_AF_NXT_NORM_W32_OFFSET,
	VCAP_AF_NXT_OFFSET_FROM_TYPE,
	VCAP_AF_NXT_TYPE_AFTER_OFFSET,
	VCAP_AF_OAM_IP_BFD_ENA,
	VCAP_AF_OAM_TWAMP_ENA,
	VCAP_AF_OAM_Y1731_SEL,
	VCAP_AF_PAG_OVERRIDE_MASK,
	VCAP_AF_PAG_VAL,
	VCAP_AF_PCP_ENA,
	VCAP_AF_PCP_VAL,
	VCAP_AF_PIPELINE_ACT_SEL,
	VCAP_AF_PIPELINE_FORCE_ENA,
	VCAP_AF_PIPELINE_PT,
	VCAP_AF_PIPELINE_PT_REDUCED,
	VCAP_AF_POLICE_ENA,
	VCAP_AF_POLICE_IDX,
	VCAP_AF_POLICE_REMARK,
	VCAP_AF_PORT_MASK,
	VCAP_AF_PTP_MASTER_SEL,
	VCAP_AF_QOS_ENA,
	VCAP_AF_QOS_VAL,
	VCAP_AF_REW_CMD,
	VCAP_AF_RLEG_DMAC_CHK_DIS,
	VCAP_AF_RLEG_STAT_IDX,
	VCAP_AF_RSDX_ENA,
	VCAP_AF_RSDX_VAL,
	VCAP_AF_RSVD_LBL_VAL,
	VCAP_AF_RT_DIS,
	VCAP_AF_RT_SEL,
	VCAP_AF_S2_KEY_SEL_ENA,
	VCAP_AF_S2_KEY_SEL_IDX,
	VCAP_AF_SAM_SEQ_ENA,
	VCAP_AF_SIP_IDX,
	VCAP_AF_SWAP_MAC_ENA,
	VCAP_AF_TCP_UDP_DPORT,
	VCAP_AF_TCP_UDP_ENA,
	VCAP_AF_TCP_UDP_SPORT,
	VCAP_AF_TC_ENA,
	VCAP_AF_TC_LABEL,
	VCAP_AF_TPID_SEL,
	VCAP_AF_TTL_DECR_DIS,
	VCAP_AF_TTL_ENA,
	VCAP_AF_TTL_LABEL,
	VCAP_AF_TTL_UPDATE_ENA,
	VCAP_AF_TYPE,
	VCAP_AF_VID_VAL,
	VCAP_AF_VLAN_POP_CNT,
	VCAP_AF_VLAN_POP_CNT_ENA,
	VCAP_AF_VLAN_PUSH_CNT,
	VCAP_AF_VLAN_PUSH_CNT_ENA,
	VCAP_AF_VLAN_WAS_TAGGED,
};

#endif /* __VCAP_AG_API__ */
