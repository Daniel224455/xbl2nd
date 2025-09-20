"""
/*******************************************************************************
 Qualcomm Proprietary Design Data
 Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.
 All rights reserved.
*******************************************************************************/
/*==============================================================================
$Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/SDMPkg/1000/Settings/DSF/tools/scripts/dsf.py#1 $
$DateTime: 2021/04/29 02:01:56 $
$Author: pwbldsvc $
==============================================================================*/
"""

# This file contains a command line UI for DSF tools and a DSF class

# GLOBAL_USES_UNDERSCORE_AND_UPPERCASE
# local_uses_underscore_and_lowercase
# membersUseCamelCase
# FunctionsUseCamelCaseStartingWithCapital

import time

string = "Import modules"
print("[STATUS] {:s} - Start".format(string))
start = time.time()
import re
import struct
import sys
import glob
import os
import stat
end = time.time()
print("[STATUS] {:s} - End".format(string))
print("[STATUS] {:s} - Time {:d}ms".format(string, int((end - start) * 1000)))

DEBUG = False

class DSF:
    # one-time class variables
    binaries_ordered_by_header_index = None
    globals = None
    def __init__(self, filename=None, target_folder=None):
        """
        Operates on CDT file.

        Arguments:
        filename    --  name of CDT file
        """
        # this class relies of relative paths
        # need to cd into file dir for relative paths to match
        os.chdir(os.path.dirname(os.path.abspath(__file__)))
        self.filename = filename
        if filename is not None:
            print("[INFO] Using CDT {:s}".format(os.path.basename(filename)))
            import xlrd
            self.dsfWorkbook = xlrd.open_workbook(self.filename)
        else:
            self.dsfWorkbook = None
        
        self.autoDict = {}
        self.absDict = {}
        self.relDict = {}
        self.sheetDictcprefixs = {} # check
        self.dcbBinDict = {}
        self.regDict = {}
        self.hwioBaseDict = {}
        self.trainingSheetDict = {}
        self.trainingDict = {}
        self.scorecardDict = {}
        self.freqDict ={}
        self.configmonDict = {}
        self.absDict_from_T32 = {} # check
        self.abs_Add_value_dict = {} # check
        # set up one-time class variables if they have not been set yet
        if DSF.globals is None:
            DSF.create_globals(target_folder)
        if DSF.binaries_ordered_by_header_index is None:
            DSF.binaries_order_elements_function()
        self.error = False
    
    @staticmethod
    def bytes_crc32(bytes):
        CRC_POLYNOMIAL = 0x04C11DB7
        CRC_INITVAL = 0xFFFFFFFF
        CRC_XOROUT = 0xFFFFFFFF

        result = CRC_INITVAL
        for octet in bytes:
            if type(octet) is str:
                octet = struct.unpack("B", octet)[0]
            for bit in range(0,8):
                if (octet >> 7) ^ (result >> 31):
                    result = ((result << 1) & 0xFFFFFFFF) ^ CRC_POLYNOMIAL
                else:
                    result = (result << 1) & 0xFFFFFFFF
                octet = (octet << 1) & 0xFF

        result = result ^ CRC_XOROUT
        return result
    
    @staticmethod
    def binaries_order_elements_function():
        #print(DSF.globals["FILES"])
        with open(DSF.globals["FILES"]["ddrss.h"],"r") as dcb_header_fp:
            struct_flag=0
            DSF.binaries_ordered_by_header_index = []
            for line in dcb_header_fp:
                line=((re.sub('[\(\)\{\}<>]', '  ',line)).strip("#\n\t")).split(' ')#.split("., ")
                line=list(filter(None, line))
                if line !=[] and line[0] == 'enum':
                    struct_flag = 1
                elif line !=[] and struct_flag >=5 and line[0].find("DCB_IDX_MAX")==-1:
                    header_text = re.sub('_IDX','',line[0])
                    header_text = re.sub(',','',header_text)
                    header_text = re.sub('_CFG','',header_text)
                    DSF.binaries_ordered_by_header_index.append(header_text)
                    struct_flag += 1
                elif line !=[] and line[0].find("DCB_IDX_MAX")!=-1:
                    break
                elif struct_flag >0:
                    struct_flag += 1
                    
    @staticmethod
    def create_globals(target_folder):
        DSF.globals = {}
        DSF.globals["PERFORCE_PATHS"] = True
        DSF.globals["DSF_MAJOR_VERSION"] = 0
        DSF.globals["DSF_BRANCH_NUMBER"] = 0
        DSF.globals["DSF_REV_IN_BRANCH"] = 0
        DSF.globals["DSF_MAJOR_VERSION_SHIFT"] = 0
        DSF.globals["DSF_BRANCH_NUMBER_SHIFT"] = 0
        DSF.globals["DCB_MAX_SIZE"] = 0
        DSF.globals["SHRM_DUMP_SIZE"] = 0
        DSF.globals["DDRSS_SAVE_RESTORE_MAX_DATA_PCH"] = 0
        DSF.globals["DDRSS_SAVE_RESTORE_MAX_ADDR"] = 0
        DSF.globals["NUM_CH"] = 0

        """
        Maps platform name to platform ID
        """
        DSF.globals["platformIdDict"] = {}
        DSF.globals["platformIdDict"]["SDM845"] = "6000"
        DSF.globals["platformIdDict"]["SDM830"] = "6001"
        DSF.globals["platformIdDict"]["SDx24"] = "6002"
        DSF.globals["platformIdDict"]["SDM855"] = "6003"
        DSF.globals["platformIdDict"]["SDM855A"] = "6003"
        DSF.globals["platformIdDict"]["SDM670"] = "6004"
        DSF.globals["platformIdDict"]["QCS605"] = "6005"
        DSF.globals["platformIdDict"]["SDM1000"] = "6006"
        DSF.globals["platformIdDict"]["SDM640"] = "6007"
        DSF.globals["platformIdDict"]["SDM865"] = "6008"
        DSF.globals["platformIdDict"]["SDx55"] = "600B"
        DSF.globals["platformIdDict"]["SDM680"] = "600C"

        """
        Maps platform name to platform codename
        """
        DSF.globals["platformCodenameDict"] = {}
        DSF.globals["platformCodenameDict"]["SDM845"] = "NAPALI"
        DSF.globals["platformCodenameDict"]["SDM830"] = "NAPALIQ"
        DSF.globals["platformCodenameDict"]["SDx24"] = "CHIRON"
        DSF.globals["platformCodenameDict"]["SDM855"] = "HANA"
        DSF.globals["platformCodenameDict"]["SDM855A"] = "HANA_AU"		
        DSF.globals["platformCodenameDict"]["SDM670"] = "WARLOCK"
        DSF.globals["platformCodenameDict"]["QCS605"] = "REDDRAGON"
        DSF.globals["platformCodenameDict"]["SDM1000"] = "POIPU"
        DSF.globals["platformCodenameDict"]["SDM640"] = "TALOS"
        DSF.globals["platformCodenameDict"]["SDM865"] = "KONA"
        DSF.globals["platformCodenameDict"]["SDx55"] = "HURACAN"
        DSF.globals["platformCodenameDict"]["SDM680"] = "MOOREA"
        
        DSF.globals["target"] = "target/" + target_folder + "/"
        
        sve_globals_files = {
            "ddrss_training.h": "../../../common/ddrss/header/ddrss_training.h",
            "phy_common.h": "../../../common/phy/phy32/header/phy_common.h",
            "shrm_common.h": "../../../common/ddrss/header/shrm_common.h",
            "ddrss.h": "../../../ddrsns/ddrss/header/ddrss.h",
            "phy_target.h": "../../../common/" + DSF.globals["target"] + "header/phy_target.h",
        }

        sw_globals_files = {
            "ddrss_training.h": "../../../../../Library/DSFTargetLib/common/ddrss/header/ddrss_training.h",
            "phy_common.h": "../../../../../Library/DSFTargetLib/common/phy/phy32/header/phy_common.h",
            "shrm_common.h": "../../../../../Library/DSFTargetLib/common/ddrss/header/shrm_common.h",
            "ddrss.h": "../../../../../Library/DSFTargetLib/ddrsns/ddrss/header/ddrss.h",
            "phy_target.h": "../../../../../Library/DSFTargetLib/common/" + DSF.globals["target"] + "header/phy_target.h",
        }
        
        # Check which file paths to use
        if "boot_images" in os.getcwd():
            DSF.globals["PERFORCE_PATHS"] = False
        
        if DSF.globals["PERFORCE_PATHS"]:
            DSF.globals["FILES"] = sve_globals_files
            DSF.globals["COMMON"] = "../../../common/target/" + target_folder + "/header/"
            DSF.globals["release"] = "../../../ddrsns/target/" + target_folder + "/settings/release/"
            DSF.globals["external"] = "../../" + DSF.globals["target"] + "external/"
            DSF.globals["internal"] = "../../" + DSF.globals["target"] + "internal/"
            DSF.globals["sve"] = "../../sve/" + DSF.globals["target"]
        else:
            DSF.globals["FILES"] = sw_globals_files
            DSF.globals["COMMON"] = "../../../../../Library/DSFTargetLib/common/target/" + target_folder + "/header/"
            DSF.globals["release"] = "../../../release/"
            DSF.globals["external"] = "../../" + DSF.globals["target"] + "external/"
            DSF.globals["internal"] = "../../" + DSF.globals["target"] + "internal/"
        #print(DSF.globals["FILES"])  print ####
        """
        Search for the following in sequence:
            #define
            one or more spaces and/or tabs
            macro name in all caps with underscores between words
            one or more spaces and/or tabs
            formula consisting of ()+-*/ and numbers
        """
        define_re = re.compile(r'^#define[ \t]+([A-Z_]+)[ \t]+([\(\)\+\-\*\/0-9 xA-F]+)$')
        
        for filename in DSF.globals["FILES"]:
            if os.path.isfile(DSF.globals["FILES"][filename]):
                with open(DSF.globals["FILES"][filename], "r") as infile:
                    for line in infile:
                        # Remove comments
                        line = line.split("//")[0]
                        
                        # Check for a macro defined as a value or formula
                        m = define_re.match(line)
                        if m:
                            #print("{:s} = {:s}".format(m.group(1), m.group(2)))
                            
                            # Check if macro name matches any key in globals dictionary
                            for key in DSF.globals:
                                if m.group(1) == key:
                                    DSF.globals[key] = eval(m.group(2))
        
        if DEBUG:
            print(DSF.globals)
        
    def RemoveRegisterPrefix(self, block, reg):
        if self.autoDict[block]["Remove Prefix"] == 1:
            reg = reg.replace(self.autoDict[block]["C Code HWIO Prefix"] + "_", "", 1)
        return reg
        
    def ErrorCheck(self):
        if self.error:
            print ("[INFO] Aborting due to error detected!")
            sys.exit(1)
        
    def PopulateAutoDict(self, force=False):
        """
        Populates autoDict from AUTO sheet
        """
        if len(self.autoDict) != 0 and force == False:
            return

        # Regular expressions used to prefixes with lowercase character to number replacements
        single = re.compile(r'^([A-Z_0-9]+)([a-z])([A-Z_0-9]*)[_]*<([0-9])>$')
        double = re.compile(r'^([A-Z_0-9]+)([a-z])([A-Z_0-9]*)([a-z])[_]*([A-Z_0-9]*)<([0-9])><([0-9])>$')

        # Open AUTO sheet in DSF XL file
        autoSheet = self.dsfWorkbook.sheet_by_name('AUTO')

        # Read header from AUTO sheet
        headerArray = autoSheet.row_values(0)

        # Obtain column index for each header
        colIdxDict = {}
        for colIdx in range(len(headerArray)):
            colIdxDict[headerArray[colIdx]] = colIdx
        
        # Read data from AUTO sheet
        for rowIdx in range(1, autoSheet.nrows):
            rowArray = autoSheet.row_values(rowIdx)
            
            # Create dictionary for block
            block = rowArray[colIdxDict['Block']]
            self.autoDict[block] = {}
            
            # Populate dictionary for block
            for key in colIdxDict:
                value = rowArray[colIdxDict[key]]
                if key == "Remove Prefix":
                    self.autoDict[block][key] = int(value)
                elif key == "Summary Sheet":
                    self.autoDict[block][key] = value.split(",")
                elif key == "CMM HWIO Prefixes":
                    self.autoDict[block][key] = []
                    # Perform lowercase character to number replacements if needed
                    for prefix in value.split(','):
                        # Check for prefix with one lowercase character to number replacement
                        m = single.match(prefix)
                        if m:
                            for i in range(int(m.group(4))+1):
                                self.autoDict[block][key].append(m.group(1)+str(i)+m.group(3))
                            continue
                        
                        # Check for prefix with two lower case character to number replacements
                        m = double.match(prefix)
                        if m:
                            for i in range(int(m.group(6))+1):
                                for j in range(int(m.group(7))+1):
                                    self.autoDict[block][key].append(m.group(1)+str(i)+m.group(3)+str(j)+m.group(5))
                            continue
                        
                        # Prefix does not have one or two number replacements
                        self.autoDict[block][key].append(prefix)
                else:
                    self.autoDict[block][key] = value
            
        # Create dictionary for SWC workbook versions
        self.swcWorkbookDict = {}
        for block in self.autoDict:
            if "SWC Workbook" in self.autoDict[block]: # bypass for external CDT
                swcWorkbookName = self.autoDict[block]["SWC Workbook"]
                if swcWorkbookName not in self.swcWorkbookDict:
                    self.swcWorkbookDict[swcWorkbookName] = 0

    def PopulateDcbBinDictFromFiles(self):
        # Read binary files into dictionary
        for bin in DSF.binaries_ordered_by_header_index:
        # Search external folder for binaries
            name = DSF.globals["external"] + bin + ".bin"
            
            if os.path.isfile(name):
                # Remove binary if already present
                if bin in self.dcbBinDict:
                    self.dcbBinDict.pop(bin, None)
                # Populate binary from bin file
                with open(name, "rb") as f:
                    self.dcbBinDict[bin] = f.read()
                    
        # Search internal folder for binaries            
            name = DSF.globals["internal"] + bin + ".bin"
            
            if os.path.isfile(name):
                # Remove binary if already present
                if bin in self.dcbBinDict:
                    self.dcbBinDict.pop(bin, None)
                # Populate binary from bin file
                with open(name, "rb") as f:
                    self.dcbBinDict[bin] = f.read()

    def PopulateDcbBinDictFromExt(self, dcb, binnames):
        # populate the dict from the existing dcb, override with binnames
        with open(dcb, "rb") as dcb_f:
            checksum = struct.unpack("<I", dcb_f.read(4))[0]
            dcb_size = struct.unpack("<I", dcb_f.read(4))[0]
            version = struct.unpack("<I", dcb_f.read(4))[0]

            header_offset = 12
            for bin in DSF.binaries_ordered_by_header_index:
                bin_off = struct.unpack("<H", dcb_f.read(2))[0]
                bin_size = struct.unpack("<H", dcb_f.read(2))[0]
                header_offset += 4

                #print("bin check: "+bin)
                if bin in binnames:
                    with open(binnames[bin], "rb") as bin_f:
                        self.dcbBinDict[bin] = bin_f.read()
                else:
                    dcb_f.seek(bin_off)
                    self.dcbBinDict[bin] = dcb_f.read(bin_size)
                    dcb_f.seek(header_offset)
                    
    def GetDcbFileName(self):
        cdtName = os.path.basename(self.filename)
        
        cdtAsicNamePattern = re.compile(r'([A-Za-z0-9]+)_V([1-9])_ASIC')
        cdtRumiNamePattern = re.compile(r'([A-Za-z0-9]+)_V([1-9])_RUMI')
        asic = cdtAsicNamePattern.match(cdtName)
        rumi = cdtRumiNamePattern.match(cdtName)
        
        if asic:
            platformName = asic.group(1)
            platformVersion = asic.group(2)
            targetSilicon = "1"
        elif rumi:
            platformName = rumi.group(1)
            platformVersion = rumi.group(2)
            targetSilicon = "0"
        else:
            print("[ERROR] Unable to determine DCB filename")
            self.error = True
        
        if platformName not in DSF.globals["platformIdDict"]:
            print("[ERROR] {:s} key not found in platform ID dictionary (DSF.globals[\"platformIdDict\"])".format(platformName))
            self.error = True
        self.ErrorCheck()
            
        
        filename = DSF.globals["platformIdDict"][platformName]
        filename += "_0"
        filename += platformVersion
        filename += "00_"
        filename += targetSilicon
        filename += "_dcb.bin"
        
        DSF.globals["platform_name"] = platformName
        
        return filename
    
    def CustomerDcbBinary(self, filename=None, populate_first=True, output=None):
        # Set DCB filename
        if filename == None:
            filename = self.GetDcbFileName()
        
        # Set output location
        if output is None:
            output = DSF.globals["release"]
            
        self.PopulateDcbBinDictFromExt(os.path.join(output, filename), [])
        self.PopulateDcbBinDictFromStructbSheet()
        self.UpdateRegDictFromAlcsSheet()
        self.PopulateHwioBaseDict()
        
        # Find and replace values
        dcbBlocks = {}
        for block in sorted(self.regDict.keys(), key=lambda obj: obj):
            addrOffset = self.hwioBaseDict[self.autoDict[block]["C Code Register Offset"]]
            
            # Get addresses and values from existing DCB binary sub-block
            dcbBlock = self.autoDict[block]['DCB Binary Sub-Block']
            if dcbBlock not in dcbBlocks:
                dcbBlocks[dcbBlock] = {}
                dcbBlocks[dcbBlock]['Addresses'] = []
                dcbBlocks[dcbBlock]['Values'] = []
                ba = bytearray(self.dcbBinDict[dcbBlock])
                while len(ba):
                    dcbBlocks[dcbBlock]['Values'].append((ba.pop() << 24) + (ba.pop() << 16) + (ba.pop() << 8) + ba.pop())
                    dcbBlocks[dcbBlock]['Addresses'].append((ba.pop() << 24) + (ba.pop() << 16) + (ba.pop() << 8) + ba.pop())
            
            for reg in sorted(self.regDict[block].keys(), key=lambda obj: self.regDict[block][obj]["Address"]):
                # Get index from address
                address = self.regDict[block][reg]["Address"] + addrOffset
                index = dcbBlocks[dcbBlock]['Addresses'].index(address)
                
                # Replace value using index
                recommended = self.regDict[block][reg]["Recommended"]
                dcbBlocks[dcbBlock]['Values'][index] = recommended
                
        for dcbBlock in dcbBlocks:
            self.dcbBinDict[dcbBlock] = bytes()
            while len(dcbBlocks[dcbBlock]['Addresses']):
                self.dcbBinDict[dcbBlock] += struct.pack("<I", dcbBlocks[dcbBlock]['Addresses'].pop())
                self.dcbBinDict[dcbBlock] += struct.pack("<I", dcbBlocks[dcbBlock]['Values'].pop())
        
        self.PopulateDcbBinDictFromFiles()
        self.WriteDcbBinary(filename, output)
    
    def GenerateDcbBinary(self, filename=None, populate_first=True, output=None):
        if populate_first:
            self.PopulateDcbBinDictFromSheets()
            self.PopulateDcbBinDictFromFiles()
        
        self.WriteDcbBinary(filename, output)
        
    def WriteDcbBinary(self, filename=None, output=None):
        # Set DCB filename
        if filename == None:
            filename = self.GetDcbFileName()
        
        header_max_size = 100

        # Header bytes object
        header = bytes()

        # Data bytes object
        data = bytes()

        # Data starts at end of header
        offset = header_max_size

        # Loop through each binary section of DCB
        for bin in DSF.binaries_ordered_by_header_index:
            # Update header
            size = len(self.dcbBinDict[bin])
            header += struct.pack("<H", offset)
            header += struct.pack("<H", size)
            offset += size

            # Update data
            data += self.dcbBinDict[bin]
            
            # Align each section of DCB to 4 byte boundary
            remainder = offset % 4
            if remainder != 0:
                remainder = 4 - remainder
            for _ in range(remainder):
                data += struct.pack("<B", 0)
            offset += remainder

        # DSF Version\
        header = struct.pack("<H", DSF.globals["DSF_MAJOR_VERSION"]) + header
        header = struct.pack("<H", (DSF.globals["DSF_BRANCH_NUMBER"] << DSF.globals["DSF_BRANCH_NUMBER_SHIFT"]) | DSF.globals["DSF_REV_IN_BRANCH"]) + header

        # Size of DCB including header
        size = len(data) + header_max_size
        header = struct.pack("<I", size) + header

        # CRC set to zero as placeholder for CRC calculation
        header = struct.pack("<I",0) + header

        # Pad header with zeros
        header = header.ljust(header_max_size, b'\0')

        # Combine header and data
        binary = header + data

        # Pad binary with zeros
        binary = binary.ljust(DSF.globals["DCB_MAX_SIZE"], b'\0')

        # Calculate CRC on entire DCB binary
        crc = DSF.bytes_crc32(binary)

        # Replace placeholder CRC
        binary = struct.pack("<I", crc) + binary[4:]
        
        # Print information on which XL file is being processed and resulting binary
        # Print CRC and size information
        print("[INFO] DCB {:s}, CRC 0x{:08X}, {:d}B free ({:d}/{:d} used)".format(
            os.path.basename(filename),
            crc,
            DSF.globals["DCB_MAX_SIZE"] - size,
            size,
            DSF.globals["DCB_MAX_SIZE"]))
        
        # Set output location
        if output is None:
            output = DSF.globals["release"]
        
        # Write to file
        with open(os.path.join(output, filename), "wb") as f:
            f.write(binary)

        # Write binaries generated for debug
        if DEBUG:
            for bin in DSF.binaries_ordered_by_header_index:
                # Write to file
                with open("output_{:s}.bin".format(bin), "wb") as f:
                    f.write(self.dcbBinDict[bin])

    def PopulateAbsDict(self, force=False):
        """
        Populates absDict from ABS sheet
        """
        if len(self.absDict) != 0 and force == False:
            return
        
        self.absDict = {}

        # Open sheet
        sheet = self.dsfWorkbook.sheet_by_name('ABS')

        # Loop through rows in sheet
        row_count = sheet.nrows
        for i in range(1, row_count):
            row_array = sheet.row_values(i)
            
            # Populate dictionary with register name as key and address as value
            self.absDict[row_array[1]] = int(row_array[0], 16)

    def PopulateFreqDict(self):
        freqSheet = self.dsfWorkbook.sheet_by_name("STRUCTs")
        headerArray = freqSheet.row_values(0)

        # Obtain column index for each header
        colIdxDict = {}
        for colIdx in range(len(headerArray)):
            colIdxDict[headerArray[colIdx]] = colIdx

        # Read data from sheet
        for rowIdx in range(1, freqSheet.nrows):
            rowArray = freqSheet.row_values(rowIdx)
            # Create dictionary for block name
            freq = rowArray[colIdxDict["Frequency"]]
            block = rowArray[colIdxDict["Block"]]
            reg = rowArray[colIdxDict["Register"]]
            if freq not in self.freqDict:
                self.freqDict[freq] = {}
            if block not in self.freqDict[freq]:
                self.freqDict[freq][block] = {}
            self.freqDict[freq][block][reg] = {}

            # Populate dictionary for block name
            self.freqDict[freq][block][reg]["Recommended"] = int(rowArray[colIdxDict["Recommended"]], 16)
            self.freqDict[freq][block][reg]["PoR Source"] = int(rowArray[colIdxDict["PoR Source"]], 16)

    def PopulateConfigMonDict(self, freq):
        self.PopulateFreqDict()
        self.UpdateRegDictFromAlcsSheet()
        
        # Check if freqeuncy already present
        if freq in self.configmonDict:
            return

        # Merge regDict with per frequency settings from freqDict
        import copy
        self.configmonDict[freq] = copy.deepcopy(self.regDict)
        for block in self.freqDict[freq]:
            for reg in self.freqDict[freq][block]:
                configmon_mask = int(self.configmonDict[freq][block][reg]["PoR Source"])
                freq_mask = self.freqDict[freq][block][reg]["PoR Source"]
                overlap_mask = configmon_mask & freq_mask
                merged_mask = configmon_mask | freq_mask
                #if merged_mask != 0xFFFFFFFF:
                    #print ("[WARNING] Incomplete mask : {:s} {:08X} = {:08X} | {:08X}".format(reg, merged_mask, configmon_mask, freq_mask))
                if overlap_mask != 0x0:
                    print ("[ERROR] Mask Conflict : {:s} {:08X} = {:08X} | {:08X}".format(reg, overlap_mask, configmon_mask, freq_mask))
                    self.error = True
                masked_configmon_value = self.configmonDict[freq][block][reg]["Recommended"] & configmon_mask
                masked_freq_value = self.freqDict[freq][block][reg]["Recommended"] & freq_mask
                self.configmonDict[freq][block][reg]["Recommended"] = masked_configmon_value | masked_freq_value
                self.configmonDict[freq][block][reg]["PoR Source"] = merged_mask
        
        # Check for error
        #self.ErrorCheck()

    def ReadModifyWrite(self, block, reg, mask, value):
        # Populate absolute address dictionary
        self.PopulateAbsDict()
        
        for cmm in self.autoDict[block]['CMM HWIO Prefixes']:
            reg_with_cmm = self.RegisterAbsoluteName(block, reg, cmm)
            read_value = self.t32api.readRegister(self.absDict[reg_with_cmm])
            read_value &= ~mask
            read_value |= value
            #self.t32api.writeRegister(self.absDict[reg_with_cmm], read_value, 0)
            self.t32api.sendCommand("d.s ezaxi:0x{:X} %l 0x{:X}".format(self.absDict[reg_with_cmm], read_value)) #t32api.writeRegister not working

    def GetSuspectCount(self):
        self.PopulateScorecardDict()
        self.PopulateHwioBaseDict()
        
        # Read DDR and SHUB frequencies from SHRM
        freq = self.t32api.readRegister(self.hwioBaseDict["SEQ_DDR_SS_SHRM_MEM_SHRM_OFFSET"] + 0xD8)
        #for cmm in self.autoDict["SHRM_CSR"]['CMM HWIO Prefixes']:
        #    reg_with_cmm = self.RegisterAbsoluteName(block, reg, cmm)
        #    ddr_freq = self.t32api.readRegister(self.absDict[reg_with_cmm] + 0x100E4)
        #    shub_freq = self.t32api.readRegister(self.absDict[reg_with_cmm] + 0x100EC)
        
        # Convert from kHz to MHz
        freq = freq/1000
        
        # Abort if frequency has already been tested
        if freq in self.configmonDict:
            print("[INFO] Skipping ConfigMon @ {:0.1f}MHz (frequency already checked)".format(freq))
            return
        else:
            print("[INFO] Running ConfigMon @ {:0.1f}MHz".format(freq))
        
        # Populate ConfigMon dictionary
        self.PopulateConfigMonDict(freq)
        
        # Make it possible to read PHY registers
        # DPC_REG_DPCC_PHY_XO_CBCR[CLK_ENABLE] = 1
        self.ReadModifyWrite('DPCC', 'DPCC_REG_PHY_XO_CBCR', 1, 1)
        
        # Make it possible to read QLINKPLL registers
        # DDRCC_TOP_CTRL_CFG[PLL0_SLEEP_B & PLL1_SLEEP_B] = 1
        self.ReadModifyWrite('DDR_CC', 'DDRCC_TOP_CTRL_CFG', (1 << 25) + (1 << 22), (1 << 25) + (1 << 22))
        
        # Check registers against ConfigMon dictionary
        for block in sorted(self.configmonDict[freq].keys(), key=lambda obj: obj):
            self.scorecardDict[block]["Suspect"][freq] = 0
            
            for reg in sorted(self.configmonDict[freq][block].keys(), key=lambda obj: self.configmonDict[freq][block][obj]["Address"]):
                for cmm in self.autoDict[block]["CMM HWIO Prefixes"]:
                    # Create dictionary for instance
                    if "Value" not in self.configmonDict[freq][block][reg]:
                        self.configmonDict[freq][block][reg]["Value"] = {}
                    self.configmonDict[freq][block][reg]["Value"][cmm] = {}
                    
                    # Obtain absolute address
                    reg_with_cmm = self.RegisterAbsoluteName(block, reg, cmm)
                    
                    # Check if absolute address is available
                    if reg_with_cmm not in self.absDict:
                        print("[ERROR] Address for register ({:s}) not found in ABS sheet".format(reg_with_cmm))
                        self.error = True
                        continue
                        
                    # Read value
                    try:
                        value_read = self.configmonDict[freq][block][reg]["Value"][cmm]["Read"] = self.t32api.readRegister(self.absDict[reg_with_cmm])
                    except:
                        print("[ERROR] Unable to read register ({:s})".format(reg_with_cmm))
                        self.error = True
                        continue
                    
                    # Determine if suspect
                    mask = int(self.configmonDict[freq][block][reg]["PoR Source"])
                    masked_value_recommended = self.configmonDict[freq][block][reg]["Recommended"] & mask
                    masked_value_read = value_read & mask
                    
                    self.configmonDict[freq][block][reg]["Value"][cmm]["Delta"] = (masked_value_read ^ masked_value_recommended)
                    if self.configmonDict[freq][block][reg]["Value"][cmm]["Delta"] != 0:
                        self.scorecardDict[block]["Suspect"][freq] += 1
                        
        # Check for error
        self.ErrorCheck()

    def UpdateScorecardSheet(self):
        """
        Updates SCORECARD sheet in CDT
        """
        
        # Open workbook and replace SCORECARD sheet
        print("[INFO] Writing SCORECARD sheet")
        import openpyxl
        workbook = openpyxl.load_workbook(self.filename, keep_vba=True)
        sheet = workbook.get_sheet_by_name('SCORECARD')
        idx = workbook.get_index(sheet)
        workbook.remove_sheet(sheet)
        sheet = workbook.create_sheet('SCORECARD', idx)
        sheet.sheet_properties.tabColor= "FF002060"
        
        # Write scorecard header
        freqSorted = sorted(self.configmonDict.keys(), key=lambda obj: obj)
        headerArray = ["Block", "Design Count", "Coverage", "Excluded"]
        for freq in freqSorted:
                headerArray.append("{:0.1f}".format(freq))
        sheet.append(headerArray)
        
        # Write summary scorecard
        for block in sorted(self.scorecardDict.keys(), key=lambda obj: obj):
            rowArray = []
            rowArray.append(block)
            rowArray.append(self.scorecardDict[block]["Design Count"])
            rowArray.append(self.scorecardDict[block]["Coverage"])
            rowArray.append(self.scorecardDict[block]["Excluded"])
            for freq in freqSorted:
                if freq in self.scorecardDict[block]["Suspect"]:
                    rowArray.append(self.scorecardDict[block]["Suspect"][freq])
                else:
                    rowArray.append(0)
            sheet.append(rowArray)
        sheet.append([""])
        
        # Write suspect details
        headerArray = ["Frequency", "Block", "Address", "Register", "Recommended", "PoR Source","Read", "Delta"]
        sheet.append(headerArray)
        for freq in freqSorted:
            for block in sorted(self.configmonDict[freq].keys(), key=lambda obj: obj):
                for reg in sorted(self.configmonDict[freq][block].keys(), key=lambda obj: self.configmonDict[freq][block][obj]["Address"]):
                    for cmm in sorted(self.autoDict[block]["CMM HWIO Prefixes"], key=lambda obj: obj):
                        reg_with_cmm = self.RegisterAbsoluteName(block, reg, cmm)

                        if self.configmonDict[freq][block][reg]["Value"][cmm]["Delta"] != 0:
                            rowArray = []
                            rowArray.append(freq)
                            rowArray.append(block)
                            rowArray.append("{:08X}".format(self.absDict[reg_with_cmm]))
                            rowArray.append(reg_with_cmm)
                            rowArray.append("{:08X}".format(self.configmonDict[freq][block][reg]["Recommended"]))
                            rowArray.append("{:08X}".format(self.configmonDict[freq][block][reg]["PoR Source"]))
                            rowArray.append("{:08X}".format(self.configmonDict[freq][block][reg]["Value"][cmm]["Read"]))
                            rowArray.append("{:08X}".format(self.configmonDict[freq][block][reg]["Value"][cmm]["Delta"]))
                            sheet.append(rowArray)
        
        # Save workbook
        print("[INFO] Saving CDT")
        workbook.save(self.filename)

    def RunConfigMon(self):
        print("[INFO] Importing t32api module")
        import t32api
        
        # Connect to T32
        while True:
            ipAddress = input("Type T32 host IP address and press Enter [localhost]: ")
            if ipAddress == '':
                ipAddress = "localhost"
            apiPort = input("Type T32 API port number and press Enter [20000]: ")
            if apiPort == '':
                apiPort = 20000
            self.t32api = t32api.t32api(str(ipAddress), str(apiPort))
            connected = self.t32api.connect()
            if connected:
                break
            else:
                option = ui_input_check("[ERROR] Unable to connect to T32. Try again? [y,n]: ", ['y', 'Y', 'n', 'N', ''])
                if option in ['n', 'N']:
                    return
        
        # Select option
        while True:
            if DSF.globals["PERFORCE_PATHS"]:
                # SVE menu
                print("[i] = Run ConfigMon post init (HAL_DDR_Init, single DDR frequency)")
                print("[t] = Run ConfigMon post training (all DDR frequencies)")
                print("[r] = Run ConfigMon post restore (all DDR frequencies)")
                print("[v] = Run ConfigMon post vi_ddr (VVDRV_ddr_init, single DDR frequency)")
                print("[c] = Run ConfigMon for current DDR frequency (must be static)")
                print("[s] = Save ConfigMon scorecard and quit")
                option = ui_input_check("Select an option (type the letter next to the option and press Enter): ", ['i', 't', 'r', 'v', 'c', 's'])
            else:
                # SW menu
                print("[c] = Run ConfigMon for current DDR frequency")
                print("[s] = Save ConfigMon scorecard and quit")
                option = ui_input_check("Select an option (type the letter next to the option and press Enter): ", ['c', 's'])
            
            # Current frequency
            if option == 'c':
                self.GetSuspectCount()
            elif option == 's':
                # Exit ConfigMon menu
                break
            elif option == 'i':
                # Go to HAL_DDR_Init function
                print("[INFO] Running HAL_DDR_Init")
                self.t32api.sendCommand("go HAL_DDR_Init")
                self.t32api.sendCommand("wait !run()")
                self.t32api.sendCommand("go.up")
                self.t32api.sendCommand("wait !run()")
                self.GetSuspectCount()
            elif option == 'v':
                # Go to VVDRV_ddr_init function
                print("[INFO] Running VVDRV_ddr_init")
                self.t32api.sendCommand("go.up")
                self.t32api.sendCommand("wait !run()")
                self.GetSuspectCount()
            elif option == 't' or option == 'r':
                if option == 'r':
                    # Disable forced retraining
                    self.t32api.sendCommand("V (*(pGlobal_Config)).gConfig.ddr_cfg.ddr_cfg.ddr_training_required = 1")
                    
                # Go to init_complete function
                print("[INFO] Going to init_complete")
                self.t32api.sendCommand("go init_complete")
                self.t32api.sendCommand("wait !run()")
                
                # Set configmon=1 to enter ConfigMon frequency loop
                self.t32api.sendCommand("v gTinyAxf_runConfigMon = 1")
                
                # Perform power collapse
                #print("[INFO] Performing DDRSS power collapse")
                #self.t32api.sendCommand("go VVDRV_ddr_pwrRstr")
                #self.t32api.sendCommand("wait !run()")
                #self.t32api.sendCommand("go.up")
                #self.t32api.sendCommand("wait !run()")
                
                # Loop through enabled frequencies in clock plan
                while True:
                    # Go to VVDRV_ddr_Request_Frequency function
                    self.t32api.sendCommand("go VVDRV_ddr_Request_Frequency")
                    self.t32api.sendCommand("wait !run()")
                    self.t32api.sendCommand("go.up")
                    self.t32api.sendCommand("wait !run()")
                    
                    if "= 0x1" in self.t32api.sendCommand("v gTinyAxf_runConfigMon")[2]:
                        # Run ConfigMon
                        self.GetSuspectCount()
                    else:
                        # Exit frequency loop
                        break
                
                # Exit ConfigMon menu
                break

        # Disconnect from T32
        self.t32api.disconnect()
        
        # Update scorecard sheet
        self.UpdateScorecardSheet()

    def PopulateScorecardDict(self):
        self.PopulateAbsDict()
        self.UpdateRegDictFromAlcsSheet()

        # Create scorecard dictionary with count initialized to zero
        if len(self.scorecardDict) != 0:
            return
        
        # Initialize design, coverage, and excluded counts
        for block in sorted(self.autoDict.keys(), key=lambda obj: obj):
            self.scorecardDict[block] = {}
            self.scorecardDict[block]["Design Count"] = 0
            self.scorecardDict[block]["Coverage"] = 0
            self.scorecardDict[block]["Excluded"] = 0
            self.scorecardDict[block]["Suspect"] = {}

        # Create dictionary to map prefix to block
        prefixToBlockDict = {}
        for block in self.autoDict:
            for prefix in self.autoDict[block]["CMM HWIO Prefixes"]:
                prefixToBlockDict[prefix] = block

        # Check each register for prefix match and increment count for associated block
        for reg in self.absDict:
            for prefix in sorted(prefixToBlockDict.keys(), key=lambda obj: (len(obj)), reverse=True): # long to short
                if prefix in reg:
                    self.scorecardDict[prefixToBlockDict[prefix]]["Design Count"] += 1
                    break
        
        # Count registers for each block in regDict
        for block in self.regDict:
            for reg in self.regDict[block]:
                self.scorecardDict[block]["Coverage"] += len(self.autoDict[block]["CMM HWIO Prefixes"])
                if self.regDict[block][reg]["PoR Source"] != 0xFFFFFFFF:
                    self.scorecardDict[block]["Excluded"] += len(self.autoDict[block]["CMM HWIO Prefixes"])
        
    def GetRegisterOffsetNameArray(self, line, infile, block):
        # Patterns
        p = re.compile(r'^\#define HWIO_(.+)_ADDR\(x\)\s+\(x\+0x([0-9A-Fa-f]+)\)\s*$')
        p_0_n = re.compile(r'^\#define HWIO_(.+)_ADDR\(base, n\)\s+\(base\+0x([0-9A-Fa-f]+)\*n\)\s*$')
        p_n = re.compile(r'^\#define HWIO_(.+)_ADDR\(base, n\)\s+\(base\+0x([0-9A-Fa-f]+)\+0x([0-9A-Fa-f]+)\*n\)\s*$')
        p_m = re.compile(r'^\#define HWIO_(.+)_ADDR\(base, m\)\s+\(base\+0x([0-9A-Fa-f]+)\+0x([0-9A-Fa-f]+)\*m\)\s*$')
        p_n_m = re.compile(r'^\#define HWIO_(.+)_ADDR\(base, n, m\)\s+\(base\+0x([0-9A-Fa-f]+)\+0x([0-9A-Fa-f]+)\*n\+0x([0-9A-Fa-f]+)\*m\)\s*$')
        p_m_n = re.compile(r'^\#define HWIO_(.+)_ADDR\(base, m, n\)\s+\(base\+0x([0-9A-Fa-f]+)\+0x([0-9A-Fa-f]+)\*m\+0x([0-9A-Fa-f]+)\*n\)\s*$')
        p_max_m_n = re.compile(r'^\#define HWIO_(.+)_MAX([mn])\s+([0-9]+)\s*$')
        
        # Helper function
        def max_m_n(infile):
            while True:
                line = infile.readline()
                match = p_max_m_n.match(line)
                if match:
                    mn = match.group(2)
                    max = int(match.group(3))
                    return mn, max
                
        def mn_append(infile, offset, mn_offset):
            offset_name_array = []
            
            mn, max = max_m_n(infile)
            for i in range(max + 1):
                #print("{:s} {:08X}".format(re.sub(mn, str(i), reg), offset + (i * mn_offset)))
                offset_name_array.append(["{:08X}".format(offset + (i * mn_offset)), re.sub(mn, str(i), reg)])
                
            return offset_name_array
                            
        def m_and_n_append(infile, offset, n_offset, m_offset):
            offset_name_array = []
            
            # Find number of m or n indices
            mn, max = max_m_n(infile)
            if mn == 'm':
                max_m = max
            elif mn == 'n':
                max_n = max
                
            # Find number of m or n indices
            mn, max = max_m_n(infile)
            if mn == 'm':
                max_m = max
            elif mn == 'n':
                max_n = max
                
            for m in range(max_m + 1):
                reg_m = re.sub('m', str(m), reg)
                for n in range(max_n + 1):
                    offset_name_array.append(["{:08X}".format(offset + (m * m_offset) + (n * n_offset)), re.sub('n', str(n), reg_m)])
                    
            return offset_name_array
        
        # No index
        match = p.match(line)
        if match:
            reg = self.RemoveRegisterPrefix(block, match.group(1))
            addr = int(match.group(2), 16)
            return [["{:08X}".format(addr), reg]]
        
        # offset 0 with n index
        match = p_0_n.match(line)
        if match:
            reg = self.RemoveRegisterPrefix(block, match.group(1))
            offset = 0
            mn_offset = int(match.group(2), 16)
            return mn_append(infile, offset, mn_offset)
        
        # n index
        match = p_n.match(line)
        if match:
            reg = self.RemoveRegisterPrefix(block, match.group(1))
            offset = int(match.group(2), 16)
            mn_offset = int(match.group(3), 16)
            return mn_append(infile, offset, mn_offset)
        
        # m index
        match = p_m.match(line)
        if match:
            reg = self.RemoveRegisterPrefix(block, match.group(1))
            offset = int(match.group(2), 16)
            mn_offset = int(match.group(3), 16)
            return mn_append(infile, offset, mn_offset)
            
        # n and m indices
        match = p_n_m.match(line)
        if match:
            reg = self.RemoveRegisterPrefix(block, match.group(1))
            offset = int(match.group(2), 16)
            n_offset = int(match.group(3), 16)
            m_offset = int(match.group(4), 16)
            return m_and_n_append(infile, offset, n_offset, m_offset)
        
        # m and n indices
        match = p_m_n.match(line)
        if match:
            reg = self.RemoveRegisterPrefix(block, match.group(1))
            offset = int(match.group(2), 16)
            m_offset = int(match.group(3), 16)
            n_offset = int(match.group(4), 16)
            return m_and_n_append(infile, offset, n_offset, m_offset)
        
        #print("[WARNING] No match for line: {:s}".format(line))
        return []
            
    def UpdateRelAndAbsSheets(self):
        """
        Updates REL and ABS sheets
        """
        self.PopulateAutoDict()
        
         # Open workbook and replace REL sheet
        print("[INFO] Writing ABS and REL sheets")
        import openpyxl
        workbook = openpyxl.load_workbook(self.filename, keep_vba=True)
        relSheet = workbook.get_sheet_by_name('REL')
        idx = workbook.get_index(relSheet)
        workbook.remove_sheet(relSheet)
        relSheet = workbook.create_sheet('REL', idx)
        relSheet.sheet_properties.tabColor = "FFD0CECE"
        
        absSheet = workbook.get_sheet_by_name('ABS')
        idx = workbook.get_index(absSheet)
        workbook.remove_sheet(absSheet)
        absSheet = workbook.create_sheet('ABS', idx)
        absSheet.sheet_properties.tabColor = "FFD0CECE"
        
        # Write header
        relSheet.append(['Address', 'Register'])
        absSheet.append(['Address', 'Register'])
        
        # Extract relative addresses from each *_hwioreg.h file
        relReg = []
        absReg = []
        for block in sorted(self.autoDict.keys(), key=lambda obj: obj):
            header = self.autoDict[block]['Header']
            
            print("[INFO] Processing {:s} using {:s}".format(block, header))
            
            # Pattern to detect register name
            p_addr = re.compile(r'^\#define HWIO_{:s}_(.+)_ADDR\(.*$'.format(self.autoDict[block]["C Code HWIO Prefix"]))
            
            with open(DSF.globals["COMMON"] + header, "r") as infile:
                for line in infile:
                    line = line.strip("\n")
                    match = p_addr.match(line)
                    if match:
                        offset_reg_array = self.GetRegisterOffsetNameArray(line, infile, block)
                        for offset_reg in offset_reg_array:
                            offset = int(offset_reg[0], 16)
                            reg = offset_reg[1]
                            
                            if reg not in relReg:
                                relReg.append(reg)
                                relSheet.append(offset_reg)
                                
                            for cmm in sorted(self.autoDict[block]["CMM HWIO Prefixes"], key=lambda obj: obj):
                                reg_with_cmm = self.RegisterAbsoluteName(block, reg, cmm)
                                if reg_with_cmm not in absReg:
                                    absReg.append(reg)
                                    base = self.GetRegisterBlockBaseAddress(block, cmm)
                                    absSheet.append(["{:08X}".format(base + offset), reg_with_cmm])
                                
        # Save workbook
        print("[INFO] Saving CDT")
        workbook.save(self.filename)
        
        # Run macros and recalculate formulas
        self.RunMacrosAndRecalcFormulas()
        
    def RegisterAbsoluteName(self, block, reg, cmm):
        if self.autoDict[block]["Remove Prefix"] == 0:
            reg_with_cmm = reg.replace(self.autoDict[block]["C Code HWIO Prefix"], cmm, 1)
        else:
            reg_with_cmm = cmm + "_" + reg
        return reg_with_cmm
        
    def PopulateRelDict(self, force=False):
        """
        Populates relDict from REL sheet
        """
        if len(self.relDict) != 0 and force == False:
            return
            
        self.relDict = {}

        # Open sheet
        sheet = self.dsfWorkbook.sheet_by_name('REL')

        # Loop through rows in sheet
        row_count = sheet.nrows
        for i in range(1, row_count):
            row_array = sheet.row_values(i)
            
            # Populate dictionary with register name as key and address as value
            self.relDict[row_array[1]] = int(row_array[0], 16)

    def RunMacrosAndRecalcFormulas(self):
        print("[INFO] Running macros and recalculating formulas")
        import win32com.client
        xl = None
        xl = win32com.client.Dispatch("Excel.Application")
        xl.Workbooks.Open(Filename=os.path.abspath(self.filename))
        xl.Application.Run("HeaderMacro")
        xl.Workbooks(1).Close(SaveChanges=1)
        xl.Application.Quit()
        del xl

    def GenerateExternalCDT(self):
        print("[INFO] Generating External CDT")
        import win32com.client
        xl = None
        xl = win32com.client.Dispatch("Excel.Application")
        xl.Workbooks.Open(Filename=os.path.abspath(self.filename))
        try:
            xl.Application.Run("ProtectWorkbook")
        except:
            print("[WARNING] ProtectWorkbook macro not found")
        xl.Workbooks(1).Close(SaveChanges=1)
        xl.Application.Quit()
        del xl

    def PopulateDcbBinDictFromStructbSheet(self):
        # Open sheet with DCB binary data for structures
        sheet = self.dsfWorkbook.sheet_by_name('STRUCTb')

        # Read header from sheet
        headerArray = sheet.row_values(0)

        # Obtain column index for each header
        colIdxDict = {}
        for colIdx in range(len(headerArray)):
            colIdxDict[headerArray[colIdx]] = colIdx

        # Temporary items
        valueDict = {} # three dimensional array <block><band><value>
        active = ''
        count = 0

        # Read values from sheet
        for rowIdx in range(1, sheet.nrows):
            rowArray = sheet.row_values(rowIdx)

            # Check Block Name
            block = rowArray[colIdxDict['Block Name']]
            if block not in valueDict:
                valueDict[block] = []
                count = 0
                # Delete to replace content in DCB
                if block in self.dcbBinDict:
                    del self.dcbBinDict[block]

            # Check Structure/Array
            structure = rowArray[colIdxDict['Structure/Array']]
            if structure != active:
                valueDict[block].append([[] for i in range(len(rowArray) - colIdxDict[0] + 1)])
                active = structure
                count += 1

            # Place values into bands
            band = 0
            for colIdx in range(colIdxDict[0], len(rowArray)):
                valueDict[block][count-1][band].append(rowArray[colIdx])
                band += 1

        # Turn values into bytes
        for block in valueDict:
            if block not in self.dcbBinDict:
                self.dcbBinDict[block] = bytes()

            for structure in valueDict[block]:
                for band in structure:
                    for value in band:
                        if len(value) == 2:
                            self.dcbBinDict[block] += struct.pack('B', int(value, 16))
                        elif len(value) == 4:
                            self.dcbBinDict[block] += struct.pack('H', int(value, 16))
                        elif len(value) == 8:
                            self.dcbBinDict[block] += struct.pack('I', int(value, 16))

    def PopulateTrainingDict(self, force=False):
        """
        Populates trainingDict from TRAINING sheet
        """
        if len(self.trainingDict) != 0 and force == False:
            return
        
        # Clear dictionary
        self.trainingDict = {}

        # Open sheet
        sheet = self.dsfWorkbook.sheet_by_name('TRAINING')
        
        # Read header from sheet
        headerArray = sheet.row_values(0)

        # Obtain column index for each header
        colIdxDict = {}
        for colIdx in range(len(headerArray)):
            colIdxDict[headerArray[colIdx]] = colIdx
            
        # Arrays defining data format for each trainingDict key
        stringKeys = ["Comment"]
        hexKeys = ["Recommended", "Default", "PoR Source"]
        intKeys = ["watchdog", "Clock witch Hang", "Trained"]

        # Loop through rows in sheet
        for i in range(1, sheet.nrows):
            rowArray = sheet.row_values(i)
            
            block = rowArray[colIdxDict["Block"]]
            if block not in self.trainingDict:
                self.trainingDict[block] = {}
            
            reg = rowArray[colIdxDict["Register"]]
            if reg not in self.trainingDict[block]:
                self.trainingDict[block][reg] = {}
                
            field = rowArray[colIdxDict["Field"]]
            self.trainingDict[block][reg][field] = {}
            self.trainingDict[block][reg][field]["Mask"] = int(rowArray[colIdxDict["Mask"]], 16)
            self.trainingDict[block][reg][field]["Shift"] = int(rowArray[colIdxDict["Shift"]], 16)
            
    def UpdateTrainingSheet(self):
        """
        Updates TRAINING sheet from SWCs sheet
        """
        self.PopulateRegDictFromSwcsSheet()
        
        # Populate trained registers in trainingDict from SWCs sheet
        self.trainingDict = {}
        for block in sorted(self.regDict.keys(), key=lambda obj: obj):
            for reg in sorted(self.regDict[block].keys(), key=lambda obj: self.regDict[block][obj]["Address"]):
                if self.regDict[block][reg]["Trained"] == 1:
                    if block not in self.trainingDict:
                        self.trainingDict[block] = {}
                    self.trainingDict[block][reg] = {}
        
        # Patterns
        p_addr = re.compile(r'^\#define HWIO_(.+)_ADDR\(.*$')
        p_mask = re.compile(r'^\#define HWIO_(.+)_BMSK\s+0x([0-9A-Fa-f]+)\s*$')
        p_shift = re.compile(r'^\#define HWIO_(.+)_SHFT\s+0x([0-9A-Fa-f]+)\s*$')
        
        # Populate mask and shift for fields of registers in trainingDict from hwioreg.h files
        field_mask = "mask"
        field_shift = "shift"
        
        visited_headers = []
        for block in sorted(self.regDict.keys(), key=lambda obj: obj):
            header = self.autoDict[block]['Header']
            
            if header in visited_headers:
                continue
                
            visited_headers.append(header)
            print("[INFO] Processing {:s}".format(header))
            
            with open(DSF.globals["COMMON"] + header) as infile:
                for line in infile:
                    line = line.strip("\n")
                    match = p_addr.match(line)
                    if match:
                        field_mask = "mask"
                        field_shift = "shift"
                        offset_reg_array = self.GetRegisterOffsetNameArray(line, infile, block)
                        name = self.RemoveRegisterPrefix(block, match.group(1))
                        #print(name)
                        
                    # Process field mask
                    match = p_mask.match(line)
                    if match:
                        field_mask = match.group(1).split(name + "_")[1]
                        value_mask = int(match.group(2), 16)
                        #print("{:s} {:08X}".format(field_mask, value_mask))
                        
                    # Process field shift
                    match = p_shift.match(line)
                    if match:
                        field_shift = match.group(1).split(name + "_")[1]
                        value_shift = int(match.group(2), 16)
                        #print("{:s} {:08X}".format(field_shift, value_shift))
                    
                    # Save if both field mask and shift are known
                    if field_mask == field_shift:
                        field = field_mask
                        field_mask = "mask"
                        field_shift = "shift"
                        #print(field)
                        for block_key in self.trainingDict:
                            for offset_reg in offset_reg_array:
                                reg = offset_reg[1]
                                if reg in self.trainingDict[block_key]:
                                    self.trainingDict[block_key][reg][field] = {}
                                    self.trainingDict[block_key][reg][field]["Mask"] = value_mask
                                    self.trainingDict[block_key][reg][field]["Shift"] = value_shift
        
        # Open workbook and replace REL sheet
        print("[INFO] Writing TRAINING sheet")
        import openpyxl
        workbook = openpyxl.load_workbook(self.filename, keep_vba=True)
        sheet = workbook.get_sheet_by_name('TRAINING')
        idx = workbook.get_index(sheet)
        workbook.remove_sheet(sheet)
        sheet = workbook.create_sheet('TRAINING', idx)
        sheet.sheet_properties.tabColor = "FFFFC000"
        
        # Header
        sheet.append(["Block","Register","Field","Mask","Shift"])
        
        # Data
        for block in sorted(self.trainingDict.keys(), key=lambda obj: obj):
            for reg in sorted(self.trainingDict[block].keys(), key=lambda obj: self.regDict[block][obj]["Address"]):
                for field in self.trainingDict[block][reg]:
                    mask = "{:08X}".format(self.trainingDict[block][reg][field]["Mask"])
                    shift = "{:08X}".format(self.trainingDict[block][reg][field]["Shift"])
                    sheet.append([block, reg, field, mask, shift])
        
        # Save workbook
        print("[INFO] Saving CDT")
        workbook.save(self.filename)
        
        # Run macros and recalculate formulas
        self.RunMacrosAndRecalcFormulas()

    def GenerateTrainingValues(self, ddrStructDataFile, stacksOffset):
        """
        Generates a dictionary of values for fields of registers in training result stacks
        """
        self.PopulateTrainingDict()
        self.PopulateRegDictFromSwcsSheet()
        addrDict = self.GenerateCompressedAddressListDict(["Trained"])
        addrInfo = self.GenerateCompressedAddressListInfoArray(addrDict)
        
        # Read training result stacks from VVDRV_ddr_struct_data_*.bin file
        saveRestoreDataDict = {}
        with open(ddrStructDataFile, "rb") as infile:
            # Seek to start of training stacks
            infile.seek(stacksOffset, 0)
            
            for info in addrInfo:
                value = struct.unpack("<I", infile.read(4))[0]
                
                block = info["Block"]
                if block not in saveRestoreDataDict:
                    saveRestoreDataDict[block] = {}
                
                prefix = info["Prefix"]
                if prefix not in saveRestoreDataDict[block]:
                    saveRestoreDataDict[block][prefix] = {}
                    
                reg = info["Register"]
                if reg not in saveRestoreDataDict[block][prefix]:
                    saveRestoreDataDict[block][prefix][reg] = {}
                    
                for field in self.trainingDict[block][reg]:
                    mask = self.trainingDict[block][reg][field]["Mask"]
                    shift = self.trainingDict[block][reg][field]["Shift"]
                    saveRestoreDataDict[block][prefix][reg][field] = (value & mask) >> shift
        
        return saveRestoreDataDict

    def PopulateDcbBinDictFromTrainingSheet(self):
        """
        Populates DCB sub-blocks for training results register address lists
        """
        """
        self.PopulateTrainingDict()
        
        for block in self.trainingDict:
            dcbBlock = block + "_REGS"
            self.dcbBinDict[dcbBlock] = bytes()
            
            for reg in sorted(self.trainingDict[block].keys(), key=lambda obj: self.trainingDict[block][obj]["Index"]):
                self.dcbBinDict[dcbBlock] += struct.pack("<I", self.trainingDict[block][reg]["Address"])
            
            # Pad with 0's to fill entire address list
            stackEntriesUsed = int(len(self.dcbBinDict[dcbBlock]) / 4)
            stackEntriesMax = DSF.globals["NUM_{:s}_TRAINING_STACK".format(block)]
            if stackEntriesUsed > stackEntriesMax:
                print("[ERROR] Too many registers marked as Trained in {:s} block to save/restore ({:d} of {:d} entries used)".format(block, stackEntriesUsed, stackEntriesMax))
                self.error = True
        
        # Abort on error
        self.ErrorCheck()
        """
        
    def PopulateRegDictFromSwcsSheet(self):
        """
        Populates regDict from SWCs sheet
        """
        if len(self.regDict) != 0:
            return
        
        self.regDict = {}
        self.PopulateAutoDict()
        sheet = self.dsfWorkbook.sheet_by_name("SWCs")
        
        # Abort if sheet is empty
        if sheet.nrows == 0:
            return
        
        # Obtain column index for each header
        headerArray = sheet.row_values(len(self.swcWorkbookDict) + 1)
        colIdxDict = {}
        for colIdx in range(len(headerArray)):
            colIdxDict[headerArray[colIdx]] = colIdx

        # Read data from SWCs sheet
        for rowIdx in range(len(self.swcWorkbookDict) + 2, sheet.nrows):
            rowArray = sheet.row_values(rowIdx)

            # Get block
            block = rowArray[colIdxDict["Block"]]
            
            # Check if block in regDict
            if block not in self.regDict:
                self.regDict[block] = {}
            
            # Add register
            stringHeaders = ["Comment"]
            hexHeaders = ["Address", "Recommended", "Default", "PoR Source"]
            intHeaders = ["watchdog", "Clock Switch Hang", "Trained"]
            
            reg = rowArray[colIdxDict["Register"]]
            self.regDict[block][reg] = {}
            self.regDict[block][reg]["Merged"] = False
            
            for header in stringHeaders:
                if header in headerArray: # bypass for external CDT
                    self.regDict[block][reg][header] = rowArray[colIdxDict[header]]
            for header in hexHeaders:
                self.regDict[block][reg][header] = int(rowArray[colIdxDict[header]], 16)
            for header in intHeaders:
                if header in headerArray: # bypass for external CDT
                    self.regDict[block][reg][header] = int(rowArray[colIdxDict[header]])

    def UpdateRegDictFromAlcsSheet(self):
        """
        Updates regDict from ALCs sheet
        """
        if len(self.regDict) != 0:
            return
        
        self.PopulateRegDictFromSwcsSheet()
        
        sheet = self.dsfWorkbook.sheet_by_name("ALCs")
        
        # Read header from sheet
        headerArray = sheet.row_values(0)
        
        # Obtain column index for each header
        colIdxDict = {}
        for colIdx in range(len(headerArray)):
            colIdxDict[headerArray[colIdx]] = colIdx
        
        # Merge register values into regDict
        for rowIdx in range(1, sheet.nrows):
            rowArray = sheet.row_values(rowIdx)
            
            block = rowArray[colIdxDict["Block"]]
            reg = rowArray[colIdxDict["Register"]]
            
            swcsMask = self.regDict[block][reg]["PoR Source"]
            swcsValue = self.regDict[block][reg]["Recommended"]
            
            alcsMask = int(rowArray[colIdxDict["PoR Source"]], 16)
            alcsValue = int(rowArray[colIdxDict["Recommended"]], 16)
            
            overlapMask = swcsMask & alcsMask
            mergedMask = swcsMask | alcsMask
            
            #if (mergedMask) != 0xFFFFFFFF:
                #print ("[WARNING] Incomplete mask : {:s} {:X} = {:X} | {:X}".format(reg, mergedMask, swcsMask, alcsMask))
            if overlapMask != 0:
                print ("[ERROR] Mask Conflict : {:s} {:X} = {:X} | {:X}".format(reg, overlapMask, swcsMask, alcsMask))
                self.error = True
            
            self.regDict[block][reg]["Recommended"] = (swcsMask & swcsValue) | (alcsMask & alcsValue)
            self.regDict[block][reg]['PoR Source'] = mergedMask
                
            # Indicate register setting is the result of a merge
            self.regDict[block][reg]["Merged"] = True
        
        # Check for error
        self.ErrorCheck()
    
    def PopulateHwioBaseDict(self):
        """
        Populates hwioBaseDict from *_hwiobase.h files
        """
        if len(self.hwioBaseDict) != 0:
            return
            
        self.hwioBaseDict["0"] = 0
        p = re.compile(r'^\#define\s+(SEQ_[A-Z_0-9]+_OFFSET)\s+0x([0-9A-Fa-f]+)\s*$')
        headersVisited = []
        for block in self.autoDict:
            header = self.autoDict[block]["Header"]
            if header not in headersVisited:
                headersVisited.append(header)
                
                file = DSF.globals["COMMON"] + header.replace("reg", "base", 1)
                with open(file, "r") as infile:
                    for line in infile:
                        line.strip("\n")
                        match = p.match(line)
                        if match:
                            self.hwioBaseDict[match.group(1)] = int(match.group(2), 16)
        
    def GetRegisterBlockBaseAddress(self, block, cmm):
        self.PopulateHwioBaseDict()
        
        found = False
        cCodeHwioPrefix = self.autoDict[block]["C Code HWIO Prefix"]
        strings = ["SEQ_DDR_SS_" + cmm + "_OFFSET",
            "SEQ_DDR_SS_" + cmm + "_" + cCodeHwioPrefix + "_OFFSET",
            "SEQ_DDR_SS_" + cmm + "_" + cCodeHwioPrefix + "_HM_OFFSET",
            "SEQ_DDR_SS_" + block + "_" + cmm + "_OFFSET"]
        
        for string in strings:
            if string in self.hwioBaseDict:
                found = True
                break
        
        if found:
            return self.hwioBaseDict[string]
        else:
            print("[ERROR] Base address for {:s} instance of {:s} block not found".format(cmm, block))
            self.error = True
            for string in strings:
                print("[INFO] Checked for {:s}".format(string))
    
    def GenerateCompressedAddressListDict(self, columnHeaderArray):
        self.PopulateHwioBaseDict()
        
        # <BaseBytes> = <bytes object of base addresses for instances of register block>
        # addrDict[<BaseBytes>]["OffsetBytes"] = <bytes object of register offsets from base address>
        # addrDict[<BaseBytes>]["Block"] = <register block>
        # addrDict[<BaseBytes>]["Bases"] = <array of base addresses for instances of register block>
        # addrDict[<BaseBytes>]["Registers"] = <array of registers>
        # addrDict[<BaseBytes>]["Offsets"] = <array of register offsets from base address>
        addrDict = {}
        
        # Populate compressed address list dictionary
        for block in sorted(self.regDict.keys(), key=lambda obj: obj):
            # Get base addresses for all instances of block
            baseBytes = bytes()
            bases = []
            
            for cmm in sorted(self.autoDict[block]["CMM HWIO Prefixes"], key=lambda obj: obj):
                base = self.GetRegisterBlockBaseAddress(block, cmm)
                baseBytes += struct.pack("<H", (base >> 12) & 0xFFFF)
                bases.append(base)
            
            for reg in sorted(self.regDict[block].keys(), key=lambda obj: self.regDict[block][obj]["Address"]):
                # Check if register should be skipped
                skip = True
                for columnHeader in columnHeaderArray:					
                    if self.regDict[block][reg][columnHeader] == 1:
                        skip = False
                if skip:
                    continue
                
                if baseBytes not in addrDict:
                    addrDict[baseBytes] = {}
                    addrDict[baseBytes]["OffsetBytes"] = bytes()
                    addrDict[baseBytes]["Block"] = block
                    addrDict[baseBytes]["Bases"] = bases
                    addrDict[baseBytes]["Registers"] = []
                    addrDict[baseBytes]["Offsets"] = []
                
                # Add to addrDict
                addrDict[baseBytes]["OffsetBytes"] += struct.pack("<H", (self.regDict[block][reg]["Address"] >> 2) & 0xFFFF)
                addrDict[baseBytes]["Registers"].append(reg)
                addrDict[baseBytes]["Offsets"].append(self.regDict[block][reg]["Address"])
        
        return addrDict
        
    def GenerateCompressedAddressListBytes(self, addrDict):
        addrBytes = bytes()
        dataSize = 0
        
        for baseBytes in sorted(addrDict.keys(), key=lambda obj: obj):
            # Add number of base addresses for instances of register block
            numBases = len(addrDict[baseBytes]["Bases"])
            addrBytes += struct.pack("<B", numBases)
            
            # Add number of register offsets from base address
            numOffsets = len(addrDict[baseBytes]["Offsets"])
            addrBytes += struct.pack("<B", numOffsets)
            
            # Calcuate data size
            dataSize += numBases * numOffsets * 4
            
            # Added array of base addresses for instances of register block
            addrBytes += baseBytes
            
            # Add array of register offsets from base address
            addrBytes += addrDict[baseBytes]["OffsetBytes"]
            
        # Add terminator
        addrBytes += struct.pack("<H", 0)
       
        # Align each section of DCB to 4 byte boundary
        remainder = len(addrBytes) % 4
        if remainder != 0:
            remainder = 4 - remainder
        for _ in range(remainder):
            addrBytes += struct.pack("<B", 0)

 
        return addrBytes, dataSize
    
    def GenerateCompressedAddressListInfoArray(self, addrListDict):
        addrListInfoArray = []
        
        for baseBytes in sorted(addrListDict.keys(), key=lambda obj: obj):
            block = addrListDict[baseBytes]["Block"]
            for cmm in self.autoDict[block]["CMM HWIO Prefixes"]:
                for reg in addrListDict[baseBytes]["Registers"]:
                    reg_with_cmm = self.RegisterAbsoluteName(block, reg, cmm)
                    info = {}
                    info["Block"] = block
                    info["Prefix"] = cmm
                    info["Register"] = reg
                    addrListInfoArray.append(info)
                    
        return addrListInfoArray
    
    def PopulateDcbBinDictFromSwcsSheet(self):
        """
        Populates DCB sub-blocks for one-time settings
        """
        self.UpdateRegDictFromAlcsSheet()
        self.PopulateHwioBaseDict()
        
        # Populate CFG sub-blocks from regDict
        dcbBlocks = []
        for block in sorted(self.regDict.keys(), key=lambda obj: obj):
            addrOffset = self.hwioBaseDict[self.autoDict[block]["C Code Register Offset"]]
            dcbBlock = self.autoDict[block]["DCB Binary Sub-Block"]
            if dcbBlock not in self.dcbBinDict:
                self.dcbBinDict[dcbBlock] = bytes()
                dcbBlocks.append(dcbBlock)
            for reg in sorted(self.regDict[block].keys(), key=lambda obj: self.regDict[block][obj]["Address"]):
                if self.regDict[block][reg]["Recommended"] != self.regDict[block][reg]["Default"] or self.regDict[block][reg]["Merged"]:
                    self.dcbBinDict[dcbBlock] += struct.pack("<I", self.regDict[block][reg]["Address"] + addrOffset)
                    self.dcbBinDict[dcbBlock] += struct.pack("<I", self.regDict[block][reg]["Recommended"])
        
        # Add terminators
        for dcbBlock in dcbBlocks:
            self.dcbBinDict[dcbBlock] += struct.pack("<Q", 0)
        
        # Save & restore compressed address list
        addrDict = self.GenerateCompressedAddressListDict(["Trained"])
        addrBytes, dataSize = self.GenerateCompressedAddressListBytes(addrDict)
        addrSize = len(addrBytes)
        
        # Check data usage
        maxData = (DSF.globals["DDRSS_SAVE_RESTORE_MAX_DATA_PCH"] * DSF.globals["NUM_CH"])
        if dataSize > maxData:
            print("[ERROR] Save & restore data exceeds space by {:d} bytes".format(
                dataSize - maxData))
            self.error = True
            
        # Check address list usage
        maxAddr = DSF.globals["DDRSS_SAVE_RESTORE_MAX_ADDR"]
        if addrSize > maxAddr:
            print("[ERROR] Save & restore addresses exceed space by {:d} bytes".format(
                addrSize - maxAddr))
            self.error = True
        
        # Check for error
        self.ErrorCheck()
        
        # Print information
        print("[INFO] S&R Data {:d}B free ({:d}/{:d} used), Addr {:d}B free ({:d}/{:d} used)".format(
            maxData - dataSize, dataSize, maxData, maxAddr - addrSize, addrSize, maxAddr))
        
        # Add to dcbBinDict
        self.dcbBinDict["SAVE_RESTORE"] = addrBytes
        
        # SHRM watchdog timeout dump address list
        addr_lists = ["UNGROUPED", "CONTROLPATH"]
        dumps = ["UNGROUPED" , "CONTROLPATH" ] #, "DATAPATH", "DATAPATH", "DATAPATH"]
        offset = (len(addr_lists) + len(dumps)) * 2
        offset1 = offset
        
        # SHRM_DUMP_HEADER
        header = bytes()
        
        # Set addr_list in SHRM_DUMP_HEADER
        header += struct.pack("<H", offset1)
        
        # SHRM dump compressed address lists
        lists = bytes()
        
        # Including register if watchdog column in SWCs sheet contains 1
        addrDict = self.GenerateCompressedAddressListDict(["watchdog"])
        addrBytes1, dataSize1 = self.GenerateCompressedAddressListBytes(addrDict)  #dataSize- dump values for all channels, addrBytes- size of compressed list
        addrSize1 = len(addrBytes1)
       
        # Align start of dumps to 4 byte offset
        remainder = addrSize1 % 4
        addrSize1 += remainder  #offset for next list
 
        # Add offset for addr_list
        lists += addrBytes1
        offset += addrSize1 
        
        # Align start of dumps to 4 byte offset
        remainder = offset % 4
        offset += remainder  #offset for next list
        
        offset2 = offset  #offset for next list
        
        # SHRM dump for next list
        addrDict = self.GenerateCompressedAddressListDict(["Clock Switch Hang"])
        addrBytes2, dataSize2 = self.GenerateCompressedAddressListBytes(addrDict)
        addrSize2 = len(addrBytes2)

        # Align start of dumps to 4 byte offset
        remainder = addrSize2 % 4
        addrSize2 += remainder  #offset for next list

        
        dump_list1 = offset2 + addrSize2
        dump_list2 = dump_list1 + dataSize1

        
        # Set wdog_dump in SHRM_DUMP_HEADER
        header += struct.pack("<H", dump_list1)
        
        header += struct.pack("<H", offset2)
        
        header += struct.pack("<H", dump_list2)
        
        # Add offset for wdog_dump
        offset += dump_list2 + dataSize2  
        
        lists += addrBytes2
        
        
        
        
        # Check size
        if offset > DSF.globals["SHRM_DUMP_SIZE"]:
            print("[ERROR] SHRM dump exceeds space by {:d} bytes".format(
                offset - DSF.globals["SHRM_DUMP_SIZE"]))
            self.error = True
        
        # Check for error
        self.ErrorCheck()
        
        # Print information
        print("[INFO] SHRM Dump {:d}/{:d} used (Data {:d}B, Addr {:d}B)".format(
            offset,
            DSF.globals["SHRM_DUMP_SIZE"],
            dataSize,
            addrSize))
        
        # Add to dcbBinDict
        self.dcbBinDict["SHRM_DUMP"] = header + lists
        
    def PopulateDcbBinDictFromSheets(self):
        """
        Populated DCB sub-blocks from SWCs, STRUCTb, and TRAINING sheets
        """
        self.PopulateDcbBinDictFromSwcsSheet()
        self.PopulateDcbBinDictFromStructbSheet()
        self.PopulateDcbBinDictFromTrainingSheet()

    def UpdateRegDictFromSummarySheet(self, sheet, sheetName):
        self.PopulateRelDict()
        
        # Obtain column index for each header
        headerArray = sheet.row_values(0)
        colIdxDict = {}
        for colIdx in range(len(headerArray)):
            colIdxDict[headerArray[colIdx]] = colIdx

        # Read data from SWCs sheet
        for rowIdx in range(1, sheet.nrows):
            rowArray = sheet.row_values(rowIdx)

            # Check for start of new block
            blockName = rowArray[colIdxDict["Block Name"]]
            if blockName != "":
                # Convert from summary sheet block name to CDT block
                blockFound = False
                for block in self.autoDict:
                    if self.autoDict[block]["Summary Block Name"] == blockName:
                        blockFound = True
                        break
                if blockFound == False:
                    print("[ERROR] Block corresponding to summary sheet block name ({:s}) not found".format(blockName))
                    self.error = True
                    break
                if block not in self.regDict:
                    self.regDict[block] = {}
                continue
            
            # Add register
            single = re.compile(r'^([A-Z_0-9]+)([a-z])([A-Z_0-9]*)<([0-9]+)>$')
            double = re.compile(r'^([A-Z_0-9]+)([a-z])([A-Z_0-9]*)([a-z])([A-Z_0-9]*)_<([0-9]+)><([0-9]+)>$')
            def convertIndexRegisterName(reg):
                def rreplace(s, old, new, occurrence):
                    li = s.rsplit(old, occurrence)
                    return new.join(li)
                
                # Check for reg with one lower case character to number replacements
                m = single.match(reg)
                if m:
                    reg_single = m.group(1)+m.group(4)+m.group(3)
                    if reg_single[-1] == "_":
                        reg_single = rreplace(reg_single, '_', '', 1)
                    return reg_single
                # Check for reg with two lower case character to number replacements
                m = double.match(reg)
                if m:
                    reg_double = m.group(1)+m.group(6)+m.group(3)+m.group(7)+m.group(5)
                    if reg_double[-1] == "_":
                        reg_double = rreplace(reg_double, '_', '', 1)
                    return reg_double

                return reg
            reg = convertIndexRegisterName(rowArray[colIdxDict["Register"]])
            reg = reg.replace(self.autoDict[block]["Summary Prefix"], self.autoDict[block]["C Code HWIO Prefix"], 1)
            self.regDict[block][reg] = {}
            if reg in self.relDict:
                self.regDict[block][reg]["Address"] = self.relDict[reg]
            else:
                print("[ERROR] {:s} is not present in REL sheet".format(reg))
                self.error = True
                
            # Arrays defining data format for each regDict key
            stringKeys = ["Comment"]
            hexKeys = ["Recommended", "Default", "PoR Source"]
            intKeys = ["watchdog", "Clock Switch Hang", "Trained"]
            
            # Helper function to convert summary sheet header to regDict key (SWCs sheet header)
            def convertSummarySheetHeaderToRegDictKey(header):
                if header in ["Default", "Default value", "Default Value", "Default values"]:
                    return "Default"
                elif header in ["Recommended", "Recommended Settings", "Recommended Setting"]:
                    return "Recommended"
                return header
            
            # Add remaining regDict keys
            foundKeys = []
            for header in colIdxDict:
                value = rowArray[colIdxDict[header]]
                key = convertSummarySheetHeaderToRegDictKey(header)
                if key in stringKeys:
                    foundKeys.append(key)
                    self.regDict[block][reg][key] = value
                elif key in hexKeys:
                    if isinstance(value, str):
                        foundKeys.append(key)
                        self.regDict[block][reg][key] = int(value, 16)
                    elif isinstance(value, float) and value == 0.0:
                        foundKeys.append(key)
                        self.regDict[block][reg][key] = 0
                    else:
                        print("[ERROR] Expected hexidecimal data for {:s} column ({:s} row)".format(header, reg))
                        self.error = True
                elif key in intKeys:
                    foundKeys.append(key)
                    self.regDict[block][reg][key] = int(value)
                    
            # Check all keys were found
            def checkKeyFound(foundKeys, requiredKeys):
                for key in requiredKeys:
                    if key not in foundKeys:
                        print("[ERROR] Column for {:s} not found in {:s} sheet".format(key, sheetName))
                        self.error = True
                        
            checkKeyFound(foundKeys, stringKeys)
            checkKeyFound(foundKeys, hexKeys)
            checkKeyFound(foundKeys, intKeys)
        
            # Abort if any error occurred
            self.ErrorCheck()

    def UpdateSwcsSheetFromSummmarySheets(self):
        """
        Updates SWCs sheet from SWC summary sheets
        """
        self.PopulateAutoDict()
        self.PopulateRegDictFromSwcsSheet()
        
        # Ask if SWC version check should be bypassed to force update
        bypass = ui_input_check("Force update (bypass SWC verson check) [n/y]: ", ['y', 'Y', 'n', 'N', ''])
        
        # Get SWC versions from SWCs sheet
        sheet = self.dsfWorkbook.sheet_by_name('SWCs')
        if sheet.nrows != 0:
            for i in range(len(self.swcWorkbookDict)):
                swcWorkbookName = sheet.row_values(i)[0]
                if swcWorkbookName in self.swcWorkbookDict:
                    self.swcWorkbookDict[swcWorkbookName] = float(sheet.row_values(i)[1])
        
        # Update regDict from SWC summary sheets
        print("[INFO] Updating regDict from summary sheets")
        for swcWorkbookName in sorted(self.swcWorkbookDict.keys(), key=lambda obj: obj):
            # Open workbook
            file = DSF.globals["sve"] + "swc/" + swcWorkbookName
            if os.path.isfile(file):
                import xlrd
                try:
                    workbook = xlrd.open_workbook(file)
                except:
                    print("[ERROR] Unable to open {:s}".format(file))
                    continue
            else:
                print("[INFO] Skipped {:s} (file not found in swc folder)".format(swcWorkbookName))
                continue
            
            # Check SWC workbook version less than or equal to SWCs sheet's version
            try:
                sheet = workbook.sheet_by_name("Rev.History")
            except:
                print("[ERROR] Workbook ({:s}) does not contain Rev.History sheet".format(swcWorkbookName))
                self.error = True
                break
            
            # Get version
            version = 0
            for i in range(sheet.nrows - 1, -1, -1):
                value = sheet.row_values(i)[0]
                if value != "":
                    version = float(value)
                    break
            
            # Check version
            if bypass in ['n', 'N', ''] and version <= self.swcWorkbookDict[swcWorkbookName]:
                print("[INFO] Skipped {:s} (SWCs sheet already contains data from same or newer version)".format(swcWorkbookName))
                continue
            else:
                self.swcWorkbookDict[swcWorkbookName] = version
            
            # Reset blocks
            for block in self.autoDict:
                if self.autoDict[block]["SWC Workbook"] == swcWorkbookName:
                    self.regDict[block] = {}
                
            # Update blocks
            blocksDone = []
            for block in sorted(self.autoDict.keys(), key=lambda obj: obj):
                if self.autoDict[block]["SWC Workbook"] == swcWorkbookName:
                    #if block in self.regDict:
                    #    del self.regDict[block]
                    if block not in blocksDone:
                        print("[INFO] {:s} block updated from {:s}".format(block, swcWorkbookName))
                        blocksDone.append(block)
                        # Process summary sheets
                        for sheet in self.autoDict[block]["Summary Sheet"]:
                            self.UpdateRegDictFromSummarySheet(workbook.sheet_by_name(sheet), sheet)
        
        # Check for errors
        self.ErrorCheck()
        
        # Open workbook and replace SWCs sheet
        print("[INFO] Writing updated SWCs sheet")
        import openpyxl
        workbook = openpyxl.load_workbook(self.filename, keep_vba=True)
        sheet = workbook.get_sheet_by_name("SWCs")
        idx = workbook.get_index(sheet)
        workbook.remove_sheet(sheet)
        sheet = workbook.create_sheet("SWCs", idx)
        sheet.sheet_properties.tabColor = "FF92D050"                
        
        # Write SWC workbook versions
        for swcWorkbookName in sorted(self.swcWorkbookDict.keys(), key=lambda obj: obj):
            sheet.append([swcWorkbookName, self.swcWorkbookDict[swcWorkbookName]])
        sheet.append([""])
        
        # Write header
        sheet.append(["Block", "Address", "Register", "Recommended", "Default", "PoR Source", "Comment", "watchdog", "Clock Switch Hang", "Trained"])
        
        # Arrays defining data format for each regDict key
        hexKeys = ["Recommended", "Default", "PoR Source"]
        
        # Write data
        dataKeys = ["Recommended", "Default", "PoR Source", "Comment", "watchdog", "Clock Switch Hang", "Trained"]
        for block in sorted(self.regDict.keys(), key=lambda obj: obj):
            for reg in sorted(self.regDict[block].keys(), key=lambda obj: self.regDict[block][obj]["Address"]):
                dataArray = [block, "{:08X}".format(self.regDict[block][reg]["Address"]), reg]
                for key in dataKeys:
                    if key in hexKeys:
                        dataArray.append("{:08X}".format(self.regDict[block][reg][key]))
                    else:
                        dataArray.append(self.regDict[block][reg][key])
                sheet.append(dataArray)
        
        # Save workbook
        print("[INFO] Saving CDT")
        workbook.save(self.filename)
                    
        # Run macros and recalculate formulas        
        self.RunMacrosAndRecalcFormulas()

    def DownloadSwcWorkbooks(self):
        """
        Downloads SWC workbooks from SharePoint
        """
        import webbrowser
        import shutil
        
        # Make swc folder if it does not exist
        swc_folder = DSF.globals["sve"] + "/swc/"
        if not os.path.exists(swc_folder):
            os.makedirs(swc_folder)
        
        # Download SWC Excel workbooks and move to swc folder
        self.PopulateAutoDict()
        links_downloaded  = []
        download_folder = "C:/Users/" + os.getlogin() + "/Downloads/"
        for block in sorted(self.autoDict.keys(), key=lambda obj: obj):
            link = self.autoDict[block]['Link']
            file = self.autoDict[block]["SWC Workbook"]
            
            # Confirm link contains file
            if file not in link:
                print("[ERROR] Link ({:s}) does not point to file ({:s})".format(link, file))
                continue
                
            if link not in links_downloaded:
                # Delete file if present in download folder
                file_in_download_folder = download_folder + file
                if os.path.isfile(file_in_download_folder):
                    print("[INFO] Deleting " + file + " from download folder")
                    os.remove(file_in_download_folder)
                    # Wait for file to be removed
                    while os.path.isfile(file_in_download_folder) == True:
                       time.sleep(1)
                
                # Download file
                print("[INFO] Downloading " + file)
                webbrowser.open(link)
                links_downloaded.append(link)
                
                # Wait for download to complete
                while os.path.isfile(file_in_download_folder) == False:
                   time.sleep(1)
                
                # Copy file to swc folder
                print("[INFO] Copying " + file + " to swc folder")
                shutil.copyfile(file_in_download_folder, swc_folder + file)

def status_message_and_time(string, function, *args):
    if DEBUG:
        print("[STATUS] {:s} - Start".format(string))
        start = time.time()
    
    retval = function(*args)
    
    if DEBUG:
        end = time.time()
        print("[STATUS] {:s} - End".format(string))
        print("[STATUS] {:s} - Time {:d}ms".format(string, int((end - start) * 1000)))
        
    return retval
    
def ui_input_check(string, options):
    while True:
        option = input(string)
        if option not in options:
            print("[ERROR] Invalid option. Please try again")
        else:
            return option
            
def ui_select_internal_cdt():
    # Populate list of options and associated file paths
    index = 0
    options = []
    files = []
    for cdt in sorted(glob.glob(DSF.globals["internal"] + '*.xlsm'), key=lambda obj: obj):
        if "$" in cdt:
            continue
        print("[{:d}] = {:s}".format(index, os.path.basename(cdt)))
        options.append(str(index))
        files.append(cdt)
        index += 1
    
    # Get input
    index = int(ui_input_check("Select CDT (type the number next to the file and press Enter): ", options))
    
    return files[index]
     
def ui_generate_dcb_binaries():
    # Make DCB binaries writable
    for item in os.listdir(DSF.globals["release"]):
        if item.endswith("_dcb.bin"):
            os.chmod(DSF.globals["release"] + item, stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)
    
    # Generate DCB binaries
    for cdt in sorted(glob.glob(DSF.globals["internal"] + '*.xlsm'), key=lambda obj: obj):
        if "$" in  cdt:
            continue
        dsf = status_message_and_time("Create DSF object", DSF, cdt)
        status_message_and_time("Generate DCB binary", dsf.GenerateDcbBinary)
    
    # Needed for ui_generate_dcb_c_file
    return dsf
        
def ui_generate_dcb_c_file(dsf):
    # Make DCB C file writable
    if os.path.isfile(DSF.globals["sve"] + "VVDRV_ddr_dcb.c"):
        os.chmod(DSF.globals["sve"] + "VVDRV_ddr_dcb.c", stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)
        
    print("[INFO] Generating VVDRV_ddr_dcb.c file with preloaded DCB buffer (SVE JTAGless)")
    
    def bin_bytes_to_hex_str(fp, path):
        try:
            with open(path,"rb") as bin_fp:
                bytes = bin_fp.read()
                for byte in bytes:
                    fp.write("0x{:02X},\n".format(byte))
        except:
            fp.write("0\n")
            
    header_and_copyright  = ""
    header_and_copyright += "/*******************************************************************************\n"
    header_and_copyright += "  Qualcomm Proprietary Design Data\n"
    header_and_copyright += "  Copyright (c) 2017-2018, Qualcomm Technologies Incorporated.\n"
    header_and_copyright += "  All rights reserved.\n"
    header_and_copyright += "*******************************************************************************/\n"
    header_and_copyright += "/*==============================================================================\n"
    header_and_copyright += "$Header: //components/rel/boot.xf/3.0.1.c1/QcomPkg/SDMPkg/1000/Settings/DSF/tools/scripts/dsf.py#1 $\n"
    header_and_copyright += "$DateTime: 2021/04/29 02:01:56 $\n"
    header_and_copyright += "$Author: pwbldsvc $\n"
    header_and_copyright += "==============================================================================*/\n"
    header_and_copyright += "// This file provides DCB binaries for JTAGless operation\n"
    
    with open(DSF.globals["sve"] + "VVDRV_ddr_dcb.c", "w") as fp:
        fp.write(header_and_copyright)
        
        fp.write("#include \"ddrss.h\"\n")
        fp.write("#ifdef COMPILING_TINY_AXF\n")
        fp.write("uint8 dcb_bin[DCB_MAX_SIZE] = {\n")
        
        # V1 RUMI
        fp.write("#if defined(VI_" + DSF.globals["platformCodenameDict"][DSF.globals["platform_name"]] + "_V1) && defined(RUMI)\n")
        bin_bytes_to_hex_str(fp, DSF.globals["release"] + DSF.globals["platformIdDict"][DSF.globals["platform_name"]] + "_0100_0_dcb.bin")
        fp.write("#endif\n")
        
        # V1 ASIC
        fp.write("#if defined(VI_" + DSF.globals["platformCodenameDict"][DSF.globals["platform_name"]] + "_V1) && defined(ASIC)\n")
        bin_bytes_to_hex_str(fp, DSF.globals["release"] + DSF.globals["platformIdDict"][DSF.globals["platform_name"]] + "_0100_1_dcb.bin")
        fp.write("#endif\n")
        
        # V2 RUMI
        fp.write("#if defined(VI_" + DSF.globals["platformCodenameDict"][DSF.globals["platform_name"]] + "_V2) && defined(RUMI)\n")
        bin_bytes_to_hex_str(fp, DSF.globals["release"] + DSF.globals["platformIdDict"][DSF.globals["platform_name"]] + "_0200_0_dcb.bin")
        fp.write("#endif\n")
        
        # V2 ASIC
        fp.write("#if defined(VI_" + DSF.globals["platformCodenameDict"][DSF.globals["platform_name"]] + "_V2) && defined(ASIC)\n")
        bin_bytes_to_hex_str(fp, DSF.globals["release"] + DSF.globals["platformIdDict"][DSF.globals["platform_name"]] + "_0200_1_dcb.bin")
        fp.write("#endif\n")
        
        fp.write("};\n")
        fp.write("#else\n")
        fp.write("uint8 dcb_bin[1];\n")
        fp.write("#endif\n")

def ui_modify_dcb_binaries():
    # Make DCB binaries writable
    for item in os.listdir(DSF.globals["release"]):
        if item.endswith("_dcb.bin"):
            os.chmod(DSF.globals["release"] + item, stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)
    
    # Modify DCB binaries using external CDT
    count = 0
    for cdt in sorted(glob.glob(DSF.globals["external"] + '*.xlsx'), key=lambda obj: obj):
        if "$" in cdt:
            continue
        dsf = status_message_and_time("Create DSF object", DSF, cdt)
        status_message_and_time("Update DCB binary from external CDT", dsf.CustomerDcbBinary)
        count += 1
    
    # Modify DCB binaries using external binary files
    if count == 0:
        for dcb in sorted(glob.glob(DSF.globals["release"] + '*_dcb.bin'), key=lambda obj: obj):
            dcb_name = os.path.basename(dcb)
            dsf = status_message_and_time("Create DSF object", DSF)
            dsf.PopulateDcbBinDictFromExt(dcb, [])
            dsf.PopulateDcbBinDictFromFiles()
            dsf.WriteDcbBinary(dcb_name, DSF.globals["release"])

def ui_generate_dcb_release():
    # Make external CDTs writable
    for item in os.listdir(DSF.globals["external"]):
        if item.endswith(".xlsx"):
            os.chmod(DSF.globals["external"] + item, stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)
    
    for cdt in sorted(glob.glob(DSF.globals["internal"] + '*.xlsm'), key=lambda obj: obj):
        if "$" in cdt:
            continue
        dsf = status_message_and_time("Create DSF object", DSF, cdt)
        
        # Generate external CDTs
        # status_message_and_time("Generate external CDT", dsf.GenerateExternalCDT)
        
        # Generate DCB binaries
        status_message_and_time("Generate DCB binary", dsf.GenerateDcbBinary)
    
    # Modify DCB binaries
    ui_modify_dcb_binaries()
    
    # Generate DCB c file
    ui_generate_dcb_c_file(dsf)
    
def ui_generate_configmon():
    filename = ui_select_internal_cdt()
    dsf = status_message_and_time("Create DSF object", DSF, filename)
    status_message_and_time("Populate Run Configmon", dsf.RunConfigMon)

def ui_update_swc_sheet():
    # Select CDT
    filename = ui_select_internal_cdt()
    dsf = status_message_and_time("Create DSF object", DSF, filename)
    
    # Download SWCs
    option = ui_input_check("Download SWCs (requires Chrome as default browser) [n/y]: ", ['y', 'Y', 'n', 'N', ''])
    if option in ['y', 'Y']:
        status_message_and_time("Download SWC Workbooks", dsf.DownloadSwcWorkbooks)
    
    # Update SWCs sheet
    status_message_and_time("Update SWCs sheet", dsf.UpdateSwcsSheetFromSummmarySheets)

def ui_update_training_sheet():
    filename = ui_select_internal_cdt()
    dsf = status_message_and_time("Create DSF object", DSF, filename)
    status_message_and_time("Update TRAINING sheet", dsf.UpdateTrainingSheet)

def ui_update_addr_sheets():
    filename = ui_select_internal_cdt()
    dsf = status_message_and_time("Create DSF object", DSF, filename)
    status_message_and_time("Update REL and ABS sheets", dsf.UpdateRelAndAbsSheets)

def ui_main_menu():
    if DSF.globals["PERFORCE_PATHS"]:
        # SVE Menu
        print("[d] = Generate External CDTs & DCBs (Required for DSF Release)")
        print("[i] = Generate DCBs using Internal CDTs (Intended for Internal Debug)")
        print("[e] = Modify DCBs using External CDTs (Intended for External Debug)")
        print("[c] = Run ConfigMon")
        print("[s] = Import from SWC Summary Sheets")
        print("[t] = Update TRAINING sheet")
        print("[a] = Update REL and ABS sheets")
        option = ui_input_check("Select an option (type the letter next to the option and press Enter): ", ['d', 'i', 'e', 'c', 's', 't', 'a'])
    else:
        # SW Menu
        print("[i] = Generate DCBs using Internal CDTs (Intended for Internal Debug)")
        print("[e] = Modify DCBs using External CDTs (Intended for External Debug)")
        print("[c] = Run ConfigMon")
        option = ui_input_check("Select an option (type the letter next to the option and press Enter): ", ['i', 'e', 'c'])
        
    if option == 'd':
        ui_generate_dcb_release()
    elif option == 'i':
        dsf = ui_generate_dcb_binaries()
        if DSF.globals["PERFORCE_PATHS"]:
            option = ui_input_check("Bypass generating VVDRV_ddr_dcb.c file [n/y]: ", ['y', 'Y', 'n', 'N', ''])
            if option in ['n', 'N', '']:
                ui_generate_dcb_c_file(dsf)
    elif option == 'e':
        ui_modify_dcb_binaries()
        print("[INFO] Bypassing VVDRV_ddr_dcb.c file generation")
    elif option == 'c':
        ui_generate_configmon()
    elif option == 's':
        ui_update_swc_sheet()
    elif option == 't':
        ui_update_training_sheet()
    elif option == 'a':
        ui_update_addr_sheets()

##
# Use DSF class
##
if __name__ == "__main__":
    # Get target list
    targets = []
    if os.path.exists("../../target"):
        targets = sorted(os.listdir("../../target/"), key=lambda obj: obj)
    
    # Select target
    index = 0
    if len(targets) > 1:
        print("[A] = all (Required for DSF Release)")
        options = ['A']
        for targetName in targets:
            print("[{:d}] = {:s}".format(index, targetName))
            options.append(str(index))
            index += 1
        
        # Get input
        option = ui_input_check("Select target (type the number/char next to the target and press Enter): ", options)
        
        if option == 'A':
            # Generate DCB binaries for all targets
            for target in targets:
                DSF.create_globals(target)
                ui_generate_dcb_release()
            sys.exit(0)
        else:
            # Convert option selected to target index
            index = int(option)
    
    # Create globals
    DSF.create_globals(targets[index])
    
    # Check if menu should be provided
    if DSF.globals["PERFORCE_PATHS"]:
        ui_main_menu()
    elif DSF.globals["internal"]:
        ui_main_menu()
    else:
        ui_modify_dcb_binaries()
