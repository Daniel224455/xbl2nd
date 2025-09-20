import xml.dom.minidom
import unicodedata
import re
import sys

#---------------------------------------------------------------------------
# Class Config holds the parameters to tune the features of DeviceConfig framework
# It automatically parses the image_cfg.xml and self initializes with the desired
# parameters to tune DeviceConfig framework.
#---------------------------------------------------------------------------
#   DevcfgFeatureSet =  {
#                            ImageCfgXML        : <path/to/image_cfg.xml>
#                            devcfgXMLTagList   : [8998_XML, 8996_XML, 8998_PLATFORM_CDP_XML,...]   
#                            targetImg          : <mpss/tz/hyp/oem/adsp>
#                        }
#
class Config:
    def __init__(self, cfgxml):
        self.imageConfigXML = cfgxml
        self.parseImageCfgXML()
        
    def parseImageCfgXML(self):
        imageXML = self.imageConfigXML
        image_cfg_dom = xml.dom.minidom.parse(imageXML)
        
        #Get handle to the tags            
        mappings = image_cfg_dom.getElementsByTagName("mapping")
        tcsrSsocHwVersionAddr = image_cfg_dom.getElementsByTagName("tcsr_soc_hw_version")[0].getAttribute("addr")
        
        #SocHwVersion Dictionary
        assert(len(mappings) > 0), "<soc_hw_version> tag missing in {} file".format(imageXML)
        socHwVersionDict = {}
        for mapping in mappings :
            socHwVersionDict[mapping.getAttribute("id")]=mapping.getAttribute("soc_id")      

        #Initialise Feature set
        #self.setFeature('DEVCFG_INFO_COLLECTOR',{})              
        self.DEVCFG_CURRENT_USED_XML_TAGS=[]
        #Extract and validate tag data             
        self.targetImg=image_cfg_dom.getElementsByTagName("target_img")[0].getAttribute("value")
        #Identify target image DevCfg build tag
        self.devcfgImgBuildTag=image_cfg_dom.getElementsByTagName("devcfg_target_build_id")[0].getAttribute("value").upper()
        #DAL_CONFIG_TARGET_ID value
        self.dalConfigTargetId=image_cfg_dom.getElementsByTagName("dal_config_target_id")[0].getAttribute("value") # default='0x${MSM_ID}'
        if not self.dalConfigTargetId:
            self.dalConfigTargetId='0x${MSM_ID}'
        #Devcfg xml tag list (SOC tags)
        self.devcfgXMLTagList=socHwVersionDict.keys()
        #Devcfg xml tag list (PLATFORM tags)
        self.devcfgPlatformXMLTagList= getXmlElement(imageXML, "devcfg_platform_xml_tags", default=[])
        #OEM Parameters
        self.oemCfgBaseAddr= getXmlElement(imageXML, "oem_config_base_addr", default='')
        self.socHwVersion=socHwVersionDict
        self.tcsrSsocHwVersionAddr=tcsrSsocHwVersionAddr
        #tcsr Physical to Virtual mapping flag (1:1 or not)
        self.tcsrPhyToVirSame=image_cfg_dom.getElementsByTagName("tcsr_soc_hw_version")[0].getAttribute("direct")
        if not self.tcsrPhyToVirSame:
            self.tcsrPhyToVirSame='false'
        #MultiCfg Flag
        self.multiCfg=image_cfg_dom.getElementsByTagName("multiCfg")[0].getAttribute("value")
        if not self.multiCfg:
            self.multiCfg='true'
        #Memory OPtimisation Flag
        self.propsMemoryOptimise=image_cfg_dom.getElementsByTagName("props_memory_optimisation")[0].getAttribute("value")
        if not self.propsMemoryOptimise:
            self.propsMemoryOptimise='false'
        #Struct Size Flag
        self.structSize=image_cfg_dom.getElementsByTagName("size_of_struct")[0].getAttribute("value")
        if not self.structSize:
            self.structSize='false'
        #Hardware version specific props Flag
        self.hwVersionSpecificProps =image_cfg_dom.getElementsByTagName("hw_version_specific_props")[0].getAttribute("value")
        if not self.hwVersionSpecificProps:
            self.hwVersionSpecificProps='false'
            
        self.blobs = []
        for b in image_cfg_dom.getElementsByTagName("blob"):
            self.blobs.append({"id":b.getAttribute("id"),"name":b.getAttribute("name"),"type":b.getAttribute("type")})

        self.blobmappings = []
        for m in image_cfg_dom.getElementsByTagName("mapping"):
            self.blobmappings.append({"id":m.getAttribute("id"),"soc":m.getAttribute("soc"),"stage":m.getAttribute("stage")})

    #this is pointless, remove asap
    def getFeature(self, feature):
        return getattr(self, feature)

def getXmlElement (xmlFile, tag, default=False):
    image_cfg_dom = xml.dom.minidom.parse(xmlFile)
    try:
        result = re.sub('\s','',image_cfg_dom.getElementsByTagName(tag)[0].firstChild.data.encode("ascii")).lower()
        if ',' in result:
            result = result.split(',')
    except:
        if default != False:
            result = default
        else:
            assert(0), "ERROR : <{0}> tag missing in {1}".format(tag, xmlFile)
    return result


template = """
#include "devcfgTypes.h"

{0}

int blobTableSize = {2};

devcfgBlobTableType devcfgBlobTable[] = {{
{1}
}};
"""
if __name__ == '__main__':
    if (len(sys.argv)>3):
        print "Filename is required input."
        exit(-1)

    bloblist = ""
    externList = ""
    thisConfig = Config(sys.argv[1])
    
    thisStage = 0
    tableSize = len(thisConfig.blobs)
    for b in thisConfig.blobs:
        for m in thisConfig.blobmappings:
            if b['id'] == m['id']:
                thisStage = m['stage']
        externList = externList + "extern DALProps {};\n".format(b['name'])
        if b['type'] == '2':
            bloblist = bloblist + "\t{{.id=\"{}\",.name=\"/devcfg.bin\",.loadtype={},.stage={}}},\n".format(b['id'], b['type'], thisStage)
        else:
            bloblist = bloblist + "\t{{.id=\"{}\",.name=&{},.loadtype={},.stage={}}},\n".format(b['id'], b['name'], b['type'], thisStage)

    f=open(sys.argv[2],"w")
    f.write(template.format(externList, bloblist[:-2], tableSize))
    f.close
    exit(0)
