#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for NepomukWebMinerPlugin
#
#\**********************************************************/

# Copyright (c) 2012, Jörg Ehrichs, <Joerg.Ehrichs@gmx.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

set(PLUGIN_NAME "NepomukWebMinerPlugin")
set(PLUGIN_PREFIX "NWMP")
set(COMPANY_NAME "KDE")

# ActiveX constants:
set(FBTYPELIB_NAME NepomukWebMinerPluginLib)
set(FBTYPELIB_DESC "NepomukWebMinerPlugin 1.0 Type Library")
set(IFBControl_DESC "NepomukWebMinerPlugin Control Interface")
set(FBControl_DESC "NepomukWebMinerPlugin Control Class")
set(IFBComJavascriptObject_DESC "NepomukWebMinerPlugin IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "NepomukWebMinerPlugin ComJavascriptObject Class")
set(IFBComEventSource_DESC "NepomukWebMinerPlugin IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 2cf19721-f0bc-5be0-92ee-2aa5bf6e5324)
set(IFBControl_GUID 81a238e1-51b9-5da6-9996-e91105489710)
set(FBControl_GUID c94baa99-7e9f-5efb-83cc-e3764f3f0492)
set(IFBComJavascriptObject_GUID ade2ba71-e275-54d9-9f92-93627c6e33ab)
set(FBComJavascriptObject_GUID 977735a7-54db-5c0d-aee5-4a69622879b3)
set(IFBComEventSource_GUID 97e0f94b-02b3-50ab-a478-00cf105a2095)

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "KDE.NepomukWebMinerPlugin")
set(MOZILLA_PLUGINID "kde.org/NepomukWebMinerPlugin")

# strings
set(FBSTRING_CompanyName "KDE")
set(FBSTRING_PluginDescription "Extract meta data from the webpage and put it into nepomuk")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2012 KDE")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "NepomukWebMinerPlugin")
set(FBSTRING_FileExtents "")
set(FBSTRING_PluginName "NepomukWebMinerPlugin")
set(FBSTRING_MIMEType "application/x-nepomukwebminerplugin")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 0)
set(FBMAC_USE_COCOA 0)
set(FBMAC_USE_COREGRAPHICS 0)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
