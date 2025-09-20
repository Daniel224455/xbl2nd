from optparse import OptionParser
import MapParser
import sys, re

################################################################################
# main
################################################################################
def main():
    # data containers
    symbols = []    # list of symbols parsed from map file
    regions = {}    # 2-level map of memory region to data segments to symbols
    segments = {}   # 2-level map of data segments to memery regions to symbols
    lib_sizes = {}  # 2-level map of lib name to segment type to size in image

    # handle command line
    usage = "usage: Mapit.py [options] <.map file>"
    parser = OptionParser(usage)

    parser.add_option("--ld", 
                      action="store", type="string", dest="ld_file",
                      help=".ld file to determine named section types from")

    parser.add_option("-x", "--xml",
                      action="store", type="string", dest="xml_file",
                      help=".xml file with HW memory regions")

    parser.add_option("-c", "--compiler",
                      action="store", type="string", dest="compiler",
                      default="GNU",
                      help="Name of compiler used to generate .map file" + \
                           "[ARM, GNU, CLANG]")

    parser.add_option("-r", "--range", action="append",
                      help="Add a memory range of interest. Format=" + \
                            "<start_address>:<size>")

    parser.add_option("-l", "--lib", action="append",
                      help="Add a lib of interest")

    (options, args) = parser.parse_args()

    # check that .map file has been provided
    if len(args) != 1:
        parser.print_help()
        sys.exit(1)

    # determine compiler that generated files
    compiler = get_compiler(options.compiler)
    if (compiler == None):
        sys.exit(2)

    # add any ranges added to regions
    if (add_ranges(options.range, regions)):
        sys.exit(3)

    # pull information from .map file
    MapParser.parse_map(args[0], symbols, compiler)

    # if ld file provided, extract named section types
    if (options.ld_file):
        MapParser.parse_ld(args[1], None, compiler)

    # if xml file is provided, extract memory regions
    if (options.xml_file):
        MapParser.parse_xml(options.xml_file, regions, compiler)

    # fill region map with symbols from .map file
    build_maps(symbols, regions, segments, options.lib)

    # calculate data segment sizes for each lib
    calc_lib_sizes(symbols, regions, lib_sizes)
 
    # DEBUG PRINT
    #print_totals(lib_sizes)
    print_libs(lib_sizes)
    print_map(regions, print_region_header)
    #print_map(segments, print_segment_header)
   
################################################################################
# get_compiler
################################################################################
def get_compiler(c):
    if (c == "ARM"):
        import Parser_ARM
        return Parser_ARM
    elif c == "GNU":
        import Parser_GNU
        return Parser_GNU
    elif c == "CLANG":
        import Parser_CLANG
        return Parser_CLANG
    else:
        print "Invalid Compiler"
        return None

################################################################################
# add_ranges
################################################################################
def add_ranges(ranges, regions):
    # exit if no ranges defined
    if ranges == None: 
        return 0

    for i, opt in enumerate(ranges):
        m = re.search('^((0x)?[0-9a-fA-F]+):((0x)?[0-9a-fA-F]+)$', opt)
        if m:
            region = MapParser.Section(opt, 
                                       int(m.group(1),0), 
                                       int(m.group(3),0))
            regions[region] = {}
        else:
            print "Invalid range format:", opt
            return -1

    return 0

################################################################################
# calc_lib_sizes
################################################################################
def calc_lib_sizes(symbols, regions, lib_sizes):
    # check each symbol and bin it appropriately
    for symbol in symbols:

        # check if valid
        if len(regions):
            in_range = False
            for region in regions:
                if (symbol.address < (region.address + region.size) and
                    region.address < (symbol.address + symbol.size)):
                        in_range = True

            # skip symbol if it doesnt fall under any memory regions
            if not in_range:
                continue

        # init new entry for lib if not existant
        if symbol.lib not in lib_sizes:
            lib_sizes[symbol.lib] = {}

        # add new array for segment if not existant
        if symbol.segment not in lib_sizes[symbol.lib]:
            lib_sizes[symbol.lib][symbol.segment] = 0

        # update size count to segment
        lib_sizes[symbol.lib][symbol.segment] += symbol.size

################################################################################
# build_maps               
################################################################################
def build_maps(symbols, regions, segments, libs):
    # fill out regions map (region->segment->symbol)
    for symbol in symbols:
        
        # check if in library of interest
        if (libs and symbol.lib not in libs):
            continue
        
        # add symbol to appropriate region and segment
        for region in regions:
            if (symbol.address < (region.address + region.size) and
                region.address < (symbol.address + symbol.size)):
                if symbol.segment in regions[region]:
                    regions[region][symbol.segment].append(symbol)
                else:
                    regions[region][symbol.segment] = [symbol,]
                    segments[symbol.segment] = {}

    # fill out segments map (segment->region->symbol)
    for region in regions:
        for segment in regions[region]:
            for symbol in regions[region][segment]:
                if region not in segments[segment]:
                    segments[segment][region] = []
                segments[segment][region].append(symbol)
     

################################################################################
# print_totals
################################################################################
def print_totals(lib_sizes):
    # calculate totals
    segments = {}
    for lib in lib_sizes:
        for seg in lib_sizes[lib]:

            if seg not in segments:
                segments[seg] = 0

            segments[seg] += lib_sizes[lib][seg]

    # print totals
    print "Memory Usage:"
    for seg in segments:
        print seg, ":", segments[seg]
    print

################################################################################
# print_libs
################################################################################
def print_libs(lib_sizes):
    # total counts
    segments = {
                 "CODE" : 0,
                 "RO" : 0,
                 "RW" : 0,
                 "ZI" : 0,
                 "OTHER" : 0,
               }

    # column width setting
    col_width = 10

    # print header
    for key in sorted(segments):
        print key, column_padding(key, col_width),
    print "Total", column_padding("Total", col_width),
    print  "ROM", column_padding("ROM", col_width), "Library"
    
    # print out each lib
    for lib in sorted(lib_sizes):
        total = 0
        rom = 0
        for seg in sorted(segments):
            if seg in lib_sizes[lib]:
                total += lib_sizes[lib][seg]
                segments[seg] += lib_sizes[lib][seg]
                if (seg == "CODE" or seg == "RO" or seg == "RW"):
                    rom += lib_sizes[lib][seg]
                print lib_sizes[lib][seg], 
                print column_padding(str(lib_sizes[lib][seg]), col_width),
            else: 
                print 0, column_padding('0', col_width),
        print total, column_padding(str(total), col_width),
        print rom, column_padding(str(rom), col_width), lib
    
    # print totals
    total = 0
    rom = 0
    for seg in sorted(segments):
        print segments[seg], column_padding(str(segments[seg]), col_width),
        total += segments[seg]
        if (seg == "CODE" or seg == "RO" or seg == "RW"):
            rom += segments[seg]
    print total, column_padding(str(total), col_width), 
    print rom, column_padding(str(rom), col_width), "Overall\n"
        
################################################################################
# column_padding
################################################################################
def column_padding(string, width):
    return " " * (width - len(string)) 

################################################################################
# print_map
################################################################################
def print_map(m, header):
    empty = True
    for key in m:
        if len(m[key]):
            empty = False

    if not empty:
        header()

    for key1 in m:
        if len(m[key1]):
            print key1
            for key2 in m[key1]:
                print key2, "\t Entity \t Address \t Size \t Obj \t Lib \t Segment"
                for sym in m[key1][key2]:
                    print "\t\t", sym
    print

################################################################################
# print_region_header
################################################################################
def print_region_header():
    print "####################################################################"
    print "# Map of selected memory ranges"
    print "# Outer Level: HW Memory Regions"
    print "# Inner Level: Memory Type"
    print "####################################################################"

################################################################################
# print_segment_header
################################################################################
def print_segment_header():
    print "####################################################################"
    print "# Map of selected memory ranges"
    print "# Outer Level: Memory Type"
    print "# Inner Level: HW Memory Regions"
    print "####################################################################"

################################################################################
main()
