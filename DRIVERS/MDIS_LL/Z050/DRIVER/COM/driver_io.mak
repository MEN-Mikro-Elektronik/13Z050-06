#***************************  M a k e f i l e  *******************************
#
#         Author: dp
#          $Date: 2017/04/28 14:14:25 $
#      $Revision: 1.1 $
#
#    Description: Makefile definitions for the Z50 driver (IO mapped)
#
#---------------------------------[ History ]---------------------------------
#
#   $Log: driver_io.mak,v $
#   Revision 1.1  2017/04/28 14:14:25  DPfeuffer
#   Initial Revision
#
#-----------------------------------------------------------------------------
#   (c) Copyright 2016 by MEN mikro elektronik GmbH, Nuernberg, Germany
#*****************************************************************************

MAK_NAME=z50_io

MAK_SWITCH=$(SW_PREFIX)MAC_IO_MAPPED

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/desc$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/oss$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/dbg$(LIB_SUFFIX)	\


MAK_INCL=$(MEN_INC_DIR)/z50_drv.h	\
         $(MEN_INC_DIR)/men_typs.h	\
         $(MEN_INC_DIR)/oss.h		\
         $(MEN_INC_DIR)/mdis_err.h	\
         $(MEN_INC_DIR)/maccess.h	\
         $(MEN_INC_DIR)/desc.h		\
         $(MEN_INC_DIR)/mdis_api.h	\
         $(MEN_INC_DIR)/mdis_com.h	\
         $(MEN_INC_DIR)/ll_defs.h	\
         $(MEN_INC_DIR)/ll_entry.h	\
         $(MEN_INC_DIR)/dbg.h		\
		 $(MEN_MOD_DIR)/d302i.h     \

MAK_INP1=z50_drv$(INP_SUFFIX)

MAK_INP=$(MAK_INP1) \

