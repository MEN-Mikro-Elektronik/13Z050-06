#************************** MDIS5 device descriptor *************************
#
#        Author: ub
#         $Date: 2017/04/28 14:14:32 $
#     $Revision: 1.2 $
#
#   Description: Metadescriptor for Z50 (swapped access)
#
#****************************************************************************

Z50_SW_1  {
	#------------------------------------------------------------------------
	#	general parameters (don't modify)
	#------------------------------------------------------------------------
    DESC_TYPE        = U_INT32  1           # descriptor type (1=device)
    HW_TYPE          = STRING   Z050_SW     # hardware name of device

	#------------------------------------------------------------------------
	#	reference to base board
	#------------------------------------------------------------------------
    BOARD_NAME       = STRING   CHAMF401    # device name of baseboard
    DEVICE_SLOT      = U_INT32  0           # used slot on baseboard (0..n)

	#------------------------------------------------------------------------
	#	device parameters
	#------------------------------------------------------------------------
}
