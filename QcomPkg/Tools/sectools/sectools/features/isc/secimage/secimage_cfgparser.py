# ===============================================================================
#
#  Copyright (c) 2013-2017 Qualcomm Technologies, Inc.
#  All Rights Reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
#
# ===============================================================================

from sectools.features.isc.cfgparser import CfgParserNamedTuple
from sectools.features.isc.cfgparser import ConfigParser
from sectools.features.isc.cfgparser import auto_gen_obj_config
from sectools.features.isc.cfgparser import auto_gen_xml_config
from sectools.features.isc.cfgparser import defines as cfgparser_defines
from sectools.features.isc.cfgparser.rule import ConfigRulesManager

secimage_cfgparser = CfgParserNamedTuple(
    "2.0", ConfigRulesManager(), cfgparser_defines, auto_gen_obj_config, auto_gen_xml_config)


class SecImageConfigParser(ConfigParser):
    """ SecImage-specific ConfigParser

    It sets the default values to ConfigParser constructor arguments.
    """
    def __init__(self, config_path, overrides=None, spec_overrides=None):
        super(SecImageConfigParser, self).__init__(
            config_path, cfgparser=secimage_cfgparser, overrides=overrides,
            spec_overrides=spec_overrides)
