// ****************************************************************************** 
// Copyright     :  Copyright (C) 2018, Hisilicon Technologies Co. Ltd.
// File name     :  hipciec_dl_reg_reg_offset.h
// Department    :  CAD Development Department
// Author        :  xxx
// Version       :  1.0
// Date          :  2017/10/24
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2 
// History       :  xxx 2018/07/13 11:22:07 Create file
// ******************************************************************************

#ifndef __HIPCIEC_DL_REG_REG_OFFSET_H__
#define __HIPCIEC_DL_REG_REG_OFFSET_H__

/* HIPCIEC_DL_REG Base address of Module's Register */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE                       (0x106000)

/******************************************************************************/
/*                      HiPCIECTRL40V200 HIPCIEC_DL_REG Registers' Definitions                            */
/******************************************************************************/

#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_INIT_CYCLE_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x0)   /* the initial fc DLLP frequency */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_FC_INIT_TIMEOUT_LIMIT_REG         (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x4)   /* the limit value of initial FC in all time */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_VC_FC_INIT_ALLTIMEOUT_HANDING_REG (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x8)   /* the VC initializtion timeout handing */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ACK_LATENCY_CYCLE_G1_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xC)   /* the ack transmission latency limit for 2.5GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ACK_LATENCY_CYCLE_G2_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x10)  /* the ack transmission latency limit for 5GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ACK_LATENCY_CYCLE_G3_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x14)  /* the ack transmission latency limit for 8GT/s mod */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ACK_LATENCY_CYCLE_G4_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x18)  /* the ack transmission latency limit for 16GT/s mod */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ACK_NAKD_TLP_NUM_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x1C)  /* the limit value of received number of not acknowledged TLP. */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_DUP_ACK_NUM_LIMIT_REG         (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x20)  /* duplicate TLP received num limit. */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_NAK_TIME_LIMIT_REG            (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x24)  /* the limit value between transimitted a NAK DLLP to received the last ACK DLLP. */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_CREDIT_NULL_FOR_SET_REG           (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x28)  /* set credit null enable,count timer limit value. */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_REPLAY_CYCLE_REG                  (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x2C)  /* the replay time cycle for all mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_P_CYCLE_G1_REG       (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x30)  /* the update timer for 2.5GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_P_CYCLE_G2_REG       (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x34)  /* the update timer for 5GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_P_CYCLE_G3_REG       (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x38)  /* the update timer for 8GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_P_CYCLE_G4_REG       (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x3C)  /* the update timer for 16GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_RX_FC_UPDATE_TIME_LIMIT_REG       (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x40)  /* update DLLP timeout error vaules */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ECC_ERR_CNT_1BIT_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x44)  /* ECC error occur count */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ECC_ERR_CNT_2BIT_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x48)  /* ECC error clear */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ECC_ERR_ADDR_REG                  (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x4C)  /* ECC error address */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_LCRC_ERR_NUM_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x50)  /* the lcrc error number */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_DCRC_ERR_NUM_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x54)  /* the crc error clear signal */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_FSM_STATE_REG                 (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x58)  /* DFX signal of FSM state */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_RETRY_STR_SEQ_REG             (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x5C)  /* DFX the Seq_Num of start retry TLP */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_RETRY_STATUS_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x60)  /* DFX signal of retry buffer read/write pointer */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_CTRL_REG           (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x64)  /* apb read retry buffer address & start signal */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_DATA0_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x68)  /* apb read retry bufferdata DW0 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_DATA1_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x6C)  /* apb read retry bufferdata DW0 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_DATA2_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x70)  /* apb read retry bufferdata DW0 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_DATA3_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x74)  /* apb read retry bufferdata DW0 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_DATA4_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x78)  /* apb read retry bufferdata DW0 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_DATA5_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x7C)  /* apb read retry bufferdata DW0 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_DATA6_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x80)  /* apb read retry bufferdata DW0 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_APB_READ_RETRY_DATA7_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x84)  /* apb read retry bufferdata DW0 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_INT_STATUS_REG                 (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x88)  /* data link original intrrrupt */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_INT_MASK_REG                   (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x8C)  /* data link intrrrupt mask reg */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_INT_RO_REG                     (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x90)  /* DL block common interrupt after mask */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_INT_SET_REG                    (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x94)  /* DL block common interrupt config by CPU */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_INFINITE_CREDIT_EN_REG            (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x98)  /* when the vc initial is infinite credit,whether send UpdateFC DLLP. */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_VEDNDOR_DLLP_REG               (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x9C)  /* whether send vendor DLLP */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_RX_VENDOR_DLLP_REG                (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xA0)  
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_SRAM_ECC_CFG_REG                  (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xA4)  /* sram ecc configuration */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_APB_READ_RETRY_DONE_REG       (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xA8)  /* dl_dfx_apb_read_retry_done */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_UPDATE_TIME_CYCLE_REG         (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xAC)  /* dfx for update long req not gnt */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_CRC_INSERT_ERROR_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xB0)  /* insert crc error for tx */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_MAC_BP_TIMER_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xB4)  /* how long for mac give rdy when vld is high */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_RETRY_CNT_REG                 (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xB8)  /* count retry timer */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_FC_PH_CNT_CONFIG_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xBC)  /* ph credit increasegap */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_FC_PD_CNT_CONFIG_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xC0)  /* pd credit increasegap */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_FC_NPH_CNT_CONFIG_REG             (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xC4)  /* nph credit increasegap */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_FC_NPD_CNT_CONFIG_REG             (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xC8)  /* npd credit increasegap */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_FC_CPLH_CNT_CONFIG_REG            (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xCC)  /* cplh credit increasegap */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_FC_CPLD_CNT_CONFIG_REG            (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xD0)  /* cpld credit increasegap */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_MAC_RETRAIN_CNT_REG            (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xD4)  /* count retrain */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_MAC_RETRAIN_LIMIT_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xD8)  /* retrain limit */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_INIT_FC_SEND_EN_REG               (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xDC)  /* init fc send enable */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_DLLP_RX_COUNT_NUM_REG         (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xE0)  /* count rx dllp number */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_DLLP_TX_COUNT_NUM_REG         (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xE4)  /* count tx dllp number */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_SEND_EN_REG                   (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xE8)  /* tx send enable */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_SEQ_NUM_CHANGE_REG            (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xEC)  /* is may change seqence number */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_DLLP_TYPE_REG                 (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xF0)  /* config dllp type */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_RX_UPDATE_EN_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xF4)  /* rx update dllp enable */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_RX_NAK_COUNT_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xF8)  /* count rx nak number */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_RX_BAD_DLLP_TYPE_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0xFC)  /* bad dllp type indicate */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_ACKD_SEQ_REG                  (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x100) /* indicate ackd_seq */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_NEXT_TRANSMIT_SEQ_REG         (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x104) /* indicate next_transmit_seq */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_NEXT_RCV_SEQ_REG              (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x108) /* indicate next_rcv_seq */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_FIFO_RD_PTR_REG               (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x10C) /* fifo read ptr from software configuration */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_FIFO_26_0_REG                 (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x110) /* fifo 26bit--0 bit data */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DFX_FIFO_37_27_REG                (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x114) /* fifo 37--27 */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_ECO_DL_REG                        (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x118) /* add eco register */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FEATURE_TIMER_CFG_REG          (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x11C) /* send feature dllp timeout */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_CORRECT_ERR_CNT_CFG_REG        (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x120) /* correct error count */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_FC_UPDATE_MODE_SEL_REG         (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x124) /* dl_fc_update_mode_sel */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_NP_CYCLE_G1_REG      (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x128) /* the update timer for 2.5GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_NP_CYCLE_G2_REG      (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x12C) /* the update timer for 5GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_NP_CYCLE_G3_REG      (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x130) /* the update timer for 8GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_NP_CYCLE_G4_REG      (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x134) /* the update timer for 16GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_CPL_CYCLE_G1_REG     (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x138) /* the update timer for 2.5GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_CPL_CYCLE_G2_REG     (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x13C) /* the update timer for 5GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_CPL_CYCLE_G3_REG     (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x140) /* the update timer for 8GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_TX_FC_UPDATE_CPL_CYCLE_G4_REG     (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x144) /* the update timer for 16GT/s mode */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_INT_CE_NFE_SEL_REG             (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x148) /* Select the CE or NFE Type for Interrupt */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_NI_INT_RO_REG                  (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x14C) /* The normal interrupt status for DL */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_FE_INT_RO_REG                  (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x150) /* The fatal Error interrupt status for DL */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_CE_INT_RO_REG                  (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x154) /* The correctable Error  interrupt status for DL */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_NFE_INT_RO_REG                 (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x158) /* The nonfatal Error  interrupt status for DL */
#define HiPCIECTRL40V200_HIPCIEC_DL_REG_DL_FC_INIT_ERR_STATUS_REG         (HiPCIECTRL40V200_HIPCIEC_DL_REG_BASE + 0x15C) /* data link layer fc init timeout err status */

#endif // __HIPCIEC_DL_REG_REG_OFFSET_H__
