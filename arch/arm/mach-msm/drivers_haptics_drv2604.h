#ifndef __DRV2604_H__
#define __DRV2604_H__
/*
** ========================================================================
** Copyright 2013 Texas Instruments Inc.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
** ========================================================================
*/

/*
** File:
**     drv2604.h
**
** Description:
**     Header file for drv2604.c
**
*/
#include <linux/switch.h>
#include <linux/earlysuspend.h>

#include <linux/semaphore.h>
#include <linux/cdev.h>
#include <linux/wakelock.h>

#define HAPTICS_DEVICE_NAME "drv2604"
#define DRIVER_VERSION 		0x20130627

#define GO_BIT_POLL_INTERVAL    15
#define STANDBY_WAKE_DELAY      1

#define REAL_TIME_PLAYBACK_STRENGTH 0x46

/* Commands */
#define HAPTIC_CMDID_PLAY_SINGLE_EFFECT     0x01
#define HAPTIC_CMDID_PLAY_EFFECT_SEQUENCE   0x02
#define HAPTIC_CMDID_PLAY_TIMED_EFFECT      0x03
#define HAPTIC_CMDID_GET_DEV_ID             0x04
#define HAPTIC_CMDID_RUN_DIAG               0x05
#define HAPTIC_CMDID_REG_WRITE  	0x09
#define HAPTIC_CMDID_REG_READ   	0x0a
#define HAPTIC_CMDID_REG_SETBIT  	0x0b
#define HAPTIC_CMDID_PATTERN_RTP      0x0c
#define HAPTIC_CMDID_STOP                   0xFF

/*
** Go
*/
#define GO_REG 0x0C
#define GO     0x01
#define STOP   0x00

/*
** Status
*/
#define STATUS_REG          0x00
#define STATUS_DEFAULT      0x00
#define DIAG_RESULT_MASK    (1 << 3)
#define AUTO_CAL_PASSED     (0 << 3)
#define AUTO_CAL_FAILED     (1 << 3)
#define DIAG_GOOD           (0 << 3)
#define DIAG_BAD            (1 << 3)

#define DEV_ID_MASK 		(7 << 5)
#define DRV2604 			(4 << 5)

/*
** Mode
*/
#define MODE_REG            0x01
#define MODE_STANDBY        0x40

#define DRV260X_MODE_MASK           0x07
#define MODE_INTERNAL_TRIGGER       0
#define MODE_EXTERNAL_TRIGGER_EDGE  1
#define MODE_EXTERNAL_TRIGGER_LEVEL 2
#define MODE_PWM_OR_ANALOG_INPUT    3
#define MODE_AUDIOHAPTIC            4
#define MODE_REAL_TIME_PLAYBACK     5
#define MODE_DIAGNOSTICS            6
#define AUTO_CALIBRATION            7
#define MODE_PATTERN_RTP_ON	      	8
#define MODE_PATTERN_RTP_OFF      	9

#define MODE_STANDBY_MASK           0x40
#define MODE_READY                  1
#define MODE_SOFT_STANDBY           0 // default

#define MODE_RESET                  0x80

/*
** Real Time Playback
*/
#define REAL_TIME_PLAYBACK_REG		0x02

/*
** Library Selection
*/
#define LIBRARY_SELECTION_REG       0x03
#define LIBRARY_SELECTION_DEFAULT   0x00

#define LIBRARY_SELECTION_MASK              0x07
#define LIBRARY_SELECTION_LIBRARY_RAM       0 // default
#define LIBRARY_SELECTION_HIZ_MASK          0x10
#define LIBRARY_SELECTION_HIZ_EN            1
#define LIBRARY_SELECTION_HIZ_DIS           0

/*
** Waveform Sequencer
*/
#define WAVEFORM_SEQUENCER_REG      0x04
#define WAVEFORM_SEQUENCER_REG2     0x05
#define WAVEFORM_SEQUENCER_REG3     0x06
#define WAVEFORM_SEQUENCER_REG4     0x07
#define WAVEFORM_SEQUENCER_REG5     0x08
#define WAVEFORM_SEQUENCER_REG6     0x09
#define WAVEFORM_SEQUENCER_REG7     0x0A
#define WAVEFORM_SEQUENCER_REG8     0x0B
#define WAVEFORM_SEQUENCER_MAX      8
#define WAVEFORM_SEQUENCER_DEFAULT  0x00

/*
** OverDrive Time Offset
*/
#define OVERDRIVE_TIME_OFFSET_REG  0x0D

/*
** Sustain Time Offset, postive
*/
#define SUSTAIN_TIME_OFFSET_POS_REG 0x0E

/*
** Sustain Time Offset, negative
*/
#define SUSTAIN_TIME_OFFSET_NEG_REG 0x0F

/*
** Brake Time Offset
*/
#define BRAKE_TIME_OFFSET_REG       0x10

/*
** Rated Voltage
*/
#define RATED_VOLTAGE_REG           0x16

/*
** Overdrive Clamp Voltage
*/
#define OVERDRIVE_CLAMP_VOLTAGE_REG 0x17

/*
** Auto Calibrationi Compensation Result
*/
#define AUTO_CALI_RESULT_REG        0x18

/*
** Auto Calibration Back-EMF Result
*/
#define AUTO_CALI_BACK_EMF_RESULT_REG 0x19

/*
** Feedback Control
*/
#define FEEDBACK_CONTROL_REG        0x1A
#define FEEDBACK_CONTROL_DEVICE_TYPE_MASK        0x80
#define FEEDBACK_CONTROL_BEMF_ERM_GAIN0 0 // 0.33x
#define FEEDBACK_CONTROL_BEMF_ERM_GAIN1 1 // 1.0x
#define FEEDBACK_CONTROL_BEMF_ERM_GAIN2 2 // 1.8x
#define FEEDBACK_CONTROL_BEMF_ERM_GAIN3 3 // 4.0x

#define FEEDBACK_CONTROL_BEMF_LRA_GAIN0 0 // 5x
#define FEEDBACK_CONTROL_BEMF_LRA_GAIN1 1 // 10x
#define FEEDBACK_CONTROL_BEMF_LRA_GAIN2 2 // 20x
#define FEEDBACK_CONTROL_BEMF_LRA_GAIN3 3 // 30x

#define LOOP_RESPONSE_SLOW      (0 << 2)
#define LOOP_RESPONSE_MEDIUM    (1 << 2) // default
#define LOOP_RESPONSE_FAST      (2 << 2)
#define LOOP_RESPONSE_VERY_FAST (3 << 2)

#define FB_BRAKE_FACTOR_1X   (0 << 4) // 1x
#define FB_BRAKE_FACTOR_2X   (1 << 4) // 2x
#define FB_BRAKE_FACTOR_3X   (2 << 4) // 3x (default)
#define FB_BRAKE_FACTOR_4X   (3 << 4) // 4x
#define FB_BRAKE_FACTOR_6X   (4 << 4) // 6x
#define FB_BRAKE_FACTOR_8X   (5 << 4) // 8x
#define FB_BRAKE_FACTOR_16X  (6 << 4) // 16x
#define FB_BRAKE_DISABLED    (7 << 4)

#define FEEDBACK_CONTROL_MODE_ERM 0 // default
#define FEEDBACK_CONTROL_MODE_LRA (1 << 7)

/*
** Control1
*/
#define Control1_REG            	0x1B
#define Control1_REG_AC_COUPLE_MASK	0x20
#define Control1_REG_DRIVE_TIME_MASK	0x1f

#define STARTUP_BOOST_ENABLED   (1 << 7)
#define STARTUP_BOOST_DISABLED  (0 << 7) // default
#define AC_COUPLE_ENABLED       (1 << 5)
#define AC_COUPLE_DISABLED      (0 << 5) // default

#define DEFAULT_DRIVE_TIME      0x13

/*
** Control2
*/
#define Control2_REG            0x1C

#define IDISS_TIME_MASK         0x03
#define IDISS_TIME_VERY_SHORT   0
#define IDISS_TIME_SHORT        1
#define IDISS_TIME_MEDIUM       2 // default
#define IDISS_TIME_LONG         3

#define BLANKING_TIME_MASK          0x0C
#define BLANKING_TIME_VERY_SHORT    (0 << 2)
#define BLANKING_TIME_SHORT         (1 << 2)
#define BLANKING_TIME_MEDIUM        (2 << 2) // default
#define BLANKING_TIME_VERY_LONG     (3 << 2)

#define AUTO_RES_GAIN_MASK         0x30
#define AUTO_RES_GAIN_VERY_LOW     (0 << 4)
#define AUTO_RES_GAIN_LOW          (1 << 4)
#define AUTO_RES_GAIN_MEDIUM       (2 << 4) // default
#define AUTO_RES_GAIN_HIGH         (3 << 4)

#define SOFT_BRAKE_MASK            0x40

#define BIDIR_INPUT_MASK           0x80
#define UNIDIRECT_INPUT            (0 << 7)
#define BRAKE_STABLIZER   (1<<6)
#define BIDIRECT_INPUT             (1 << 7) // default

/*
** Control3
*/
#define Control3_REG 0x1D
#define Control3_REG_LOOP_MASK		0x21
#define Control3_REG_PWMANALOG_MASK	0x02

#define INPUT_PWM               (0 << 1) // default
#define INPUT_ANALOG            (1 << 1)
#define ERM_OpenLoop_Enabled    (1 << 5)
#define NG_Thresh_DISABLED      (0 << 6)
#define NG_Thresh_1             (1 << 6)
#define NG_Thresh_2             (2 << 6)
#define NG_Thresh_3             (3 << 6)

/*
** Auto Calibration Memory Interface
*/
#define AUTOCAL_MEM_INTERFACE_REG   0x1E
#define AUTOCAL_MEM_INTERFACE_REG_OTP_MASK   0x04

#define AUTOCAL_TIME_150MS          (0 << 4)
#define AUTOCAL_TIME_250MS          (1 << 4)
#define AUTOCAL_TIME_500MS          (2 << 4)
#define AUTOCAL_TIME_1000MS         (3 << 4)

#define SILICON_REVISION_REG        0x3B
#define SILICON_REVISION_MASK       0x07

#define MAX_TIMEOUT 10000 /* 10s */

#define SW_STATE_IDLE				0x00
#define SW_STATE_SEQUENCE_PLAYBACK		0x02
#define SW_STATE_RTP_PLAYBACK			0x04

#define	DRV2604_REG_RAM_ADDR_UPPER_BYTE	0xfd
#define	DRV2604_REG_RAM_ADDR_LOWER_BYTE	0xfe
#define	DRV2604_REG_RAM_DATA			0xff

enum actuator_type {
    ERM,
    LRA
};

enum loop_type {
    OPEN_LOOP,
    CLOSE_LOOP
};

struct actuator_data {
    enum actuator_type device_type;
    enum loop_type	loop;
    unsigned char 	rated_vol;
    unsigned char 	over_drive_vol;
    unsigned char	drive_time;
};

struct drv2604_platform_data {
    int		GpioEnable;
    int 	GpioTrigger;
    struct actuator_data actuator;
};

struct drv2604_fw_header{
    int fw_magic;
    int fw_size;
    int fw_date;
    int fw_chksum;
    int fw_effCount;
};

#define DRV2604_MAGIC	0x2604

struct drv2604_data {
    struct drv2604_platform_data PlatData;
    struct i2c_client *client;
    int	device_id;

    struct class* class;
    struct device* device;
    dev_t version;
    struct semaphore sem;
    struct cdev cdev;
    struct switch_dev sw_dev;
    int vibrator_is_playing;
    char read_val;
    char *pReadValue;
    int ReadLen;

    struct drv2604_fw_header fw_header;
    int vibration_time;
    int silence_time;
    char repeat_times;
    char mode;

#ifdef CONFIG_HAS_EARLYSUSPEND
    struct early_suspend    early_suspend;
#endif
    int OTP;

    struct hrtimer vib_timer;
    struct timed_output_dev timed_dev;
    //spinlock_t lock;
    struct mutex lock;
    struct work_struct work;
    struct device dev;
    struct pwm_device *pwm;
    int pwm_channel_id;
    int state;
    int pwm_duty_us;
    int pwm_period_us;
    int cur_duty_us;
    int max_duration;
    int init_duration;

    struct wake_lock wklock;
    struct work_struct work_play_eff;
    unsigned char sequence[8];
    volatile int should_stop;
};

#endif