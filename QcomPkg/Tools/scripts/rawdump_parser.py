#This tool is desiign for testing purpose
#split  raw dump collected form EMMC/internal partition to QPST dump minidump format and decrypt Apps/Modem/TZ Diag minidump
#create by: Vikram <dewangan@qti.qualcomm.com> 

import sys
import os
import getopt
import time
import glob
import re
import string
import struct
import subprocess
import collections
import Crypto.Util.Counter
import os.path
import sys, getopt,os
import argparse,string
import binascii
import base64
import hashlib
import hmac
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Cipher import AES
from Crypto import Random
from sys import argv
from Crypto.Signature.pss import MGF1
IV_SIZE  = 16
MAC_SIZE = 32
MODE= AES.MODE_CTR
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS) 
unpad = lambda s : s[:-ord(s[len(s)-1:])]

#Read private key and decrypt encrypted AES key
def get_AES_key(priv_key_file, enc_aes_key_file):
    #Get the private key
    rsa_key = RSA.import_key(priv_key_file.read())
    #Generate cipher
    cipher = PKCS1_OAEP.new(rsa_key, hashAlgo=SHA256)
    ciphertext = enc_aes_key_file.read()
    decrypted = cipher.decrypt(ciphertext)
    return decrypted

	
def decrypt_minidump_file(in_file,key,iv,mac,out_file):
    aes_key = key[0:32]
    print "aes_key: " + binascii.hexlify(aes_key)
    hmac_key = key[32:64]
    print "hmac_key: " + binascii.hexlify(hmac_key)
    md_ver = key[64:66]
    print "md_ver: " + binascii.hexlify(md_ver)
    print "iv_file: " + binascii.hexlify(iv)
    print "mac: " + binascii.hexlify(mac)
	#Generate HMAC for encrypted file using SHA256
    digest_maker = hmac.new(hmac_key, '', hashlib.sha256)
    while True:
        #block = in_file.read(1024)
        block = in_file.read(1024)
        if not block:
            break
        digest_maker.update(block)
    digest = digest_maker.hexdigest()
    #print "digest in hex: " +digest
    digest_mac = int(digest,32)
    #print digest_mac
    
    #print "md mac in hex: " + binascii.hexlify(mac)
    md_mac = int(mac.encode("hex"),32)
    #print md_mac
    #print "key: " + binascii.hexlify(key)
    #print "mac: " + binascii.hexlify(mac)
    #print "iv: "  + binascii.hexlify(iv)
	
    if digest_mac != md_mac:
	    print "md file incorrect skipping..."
		
    ctr = Crypto.Util.Counter.new(128, initial_value=long(iv.encode("hex"), 16))
    #print ctr
    cipher = Crypto.Cipher.AES.new(aes_key, Crypto.Cipher.AES.MODE_CTR,counter=ctr)
    in_file.seek(0)
    bs = 16
    next_chunk = ''
    finished = False
    while not finished:
        chunk,next_chunk = next_chunk, cipher.decrypt(in_file.read(bs))
        if len(next_chunk) == 0:
            finished = True
        out_file.write(chunk)	


def get_TZ_Diag_AES_key(priv_key_file, enc_aes_key_file):
    #Get the private key
    rsa_key = RSA.importKey(priv_key_file.read())
    #Generate cipher
    cipher = PKCS1_OAEP.new(rsa_key, hashAlgo=SHA256)
    enc_aes_key_file.seek(0x106C) #to read TZ diag key
    ciphertext = enc_aes_key_file.read(0x100)
    #print binascii.hexlify(ciphertext)
    decrypted = cipher.decrypt(ciphertext)
    enc_aes_key_file.seek(0)
    return decrypted

def decrypt_TZ_diag_minidump_file(in_file,key,iv,out_file):
    bs = AES.block_size
    cipher = AES.new(key, AES.MODE_GCM, iv)
    next_chunk = ''
    finished = False
    while not finished:
        chunk, next_chunk = next_chunk, cipher.decrypt(in_file.read(bs))
        if len(next_chunk) == 0:
            finished = True
        out_file.write(chunk)	

class minidump():

    diag_dict = {}
    out_folder = '.'
    out_file = None
    hw_id = '660'
    symbol = {}

    def open_file(self, name, op):
        path = os.path.join(self.out_folder, name)
        self.out_file = open(path, op)
        return self.out_file

    def split_rawdump(self, rawdump, option='file'):
        head = "<8sIIQ8sIQQI";
        head_size = struct.calcsize(head)
        section_head = "<IIIQQQQ20s";
        section_head_size = struct.calcsize(section_head)
        print("headsize:%d %d" % (head_size, section_head_size))
        filename = os.path.join("", rawdump)
        if not os.path.exists(filename):
            print "rawdump not exist"
            return False
        f = open(filename, "rb")
        head_buf = f.read(head_size)
        #create dump_info file to hold RAW dump info
        dump_info=self.open_file("dump_info.txt","w")
        (sig, version, valid, data, context, reset_trigger, dump_size, total_size, sections_count) = struct.unpack_from(head, head_buf, 0)
        if sig != "Raw_Dmp!":
            print "rawdump signal is not Raw_Dmp!"
            return False
        if valid != 1:
            print "Valid tag not set! But proceeding to split the partition"    #Romil
            #return False
        count = 0
        section_list = []
        file_list ={}

        while count < sections_count:
            section_buf = f.read(section_head_size)
            section_list.append(section_buf)
            count += 1
       
        print("sig:\t%s\nversion:\t%d\nvalid:\t%d\ndata:\t0x%lx\ncontext:\t%s\nreset_trigger:\t%x\ndump_size:\t0x%lx\nTotal Size:\t0x%lx\nSection Count:\t%d\n"

             % (sig, version, valid, data, context, reset_trigger, dump_size, total_size, sections_count))
        for i, one_section in enumerate(section_list):
            valid, version, section_type, section_offset, section_size, paddr, info, name = struct.unpack_from(section_head, one_section, 0)
            name = string.strip(name, '\0')
            file_list[name] = {}
            file_list[name]['size'] = section_size
            file_list[name]['paddr'] = paddr
            if name == 'load.cmm':
                f.seek(section_offset)
                cmmbuf = f.read(section_size)
     
            if valid != 1:
                print("%s\t section not valid: 0x%x, size:%d, valid:%d" % (name, paddr, section_size, valid ))
                print("Bypassing and proceeding to split")
                #continue   #Romil
            if option == 'split' :#or name.startswith('md_K'):
                print "Writing %s @0x%x len:0x%x ...\n" % (name, section_offset, section_size),
                fout = self.open_file(name, "wb")
                if i < sections_count :
                    dump_info.write(name + '\n')
                f.seek(section_offset)
                allread = 0
                block_size = 0x100000 * 10
                left = section_size
                size = block_size if left > block_size else left
                while left > 0:
                    buf = f.read(size)
                    fout.write(buf)
                    allread += size
                    left -= size
                    if allread % (block_size * 20) == 0:
                        print("%d MB ") % (allread / 1024 / 1024)
                    size = block_size if left > block_size else left
                fout.close()
            
        dump_info.close()
        f.close()
        return True

    def __init__(self, out_folder,output):
        if not os.path.isdir(out_folder):
            os.mkdir(out_folder)
        self.out_folder = out_folder
        if output == 0 :
            return
        if not os.path.isdir(output):
            os.mkdir(output)
        self.output = output


    def parsed_dump_decryption(self, app_priv_key, mss_priv_key, minidump, output) :
        	
        #Open RSA private XBL 
        app_pri_key_file = open(app_priv_key,'r')
        #open dump_info.txt file to read dump in sequence 
        dump_info=self.open_file("dump_info.txt","r")
	    #Open encrypted AES key file "md_encr_key_aes.BIN" and get the AES key to decrypt the minidump
        apps_enc_aes_key_file = open(os.path.join(minidump,'md_encr_key_aes.BIN'),'rb')
        apps_aes_key = get_AES_key(app_pri_key_file,apps_enc_aes_key_file)	
	    #Open Apps IV file md_encr_key_iv.BIN
        apps_iv_file = open(os.path.join(minidump,'md_encr_key_iv.BIN'), 'rb')
        #print "apps_iv_file: " + binascii.hexlify(apps_iv_file.read())
	    #Open MAC file md_encr_key_mac.BIN
        apps_mac_file  = open(os.path.join(minidump,'md_encr_key_mac.BIN'), 'rb')
        #print "apps_mac_file: " + binascii.hexlify(apps_mac_file.read())
	    
        #Ignore if request for only Apps dump decryption use only for Modem dump and TZ diag decryption with devcfg key
        if mss_priv_key != 0 :
            #open RSA private MSS key file
            mss_pri_key_file = open(mss_priv_key,'r')
			#Open encrypted AES key file "md_mss_key_aes.BIN" and get the AES key to decrypt the minidump
            mss_enc_aes_key_file = open(os.path.join(minidump,'md_mss_key_aes.BIN'),'rb')
            mss_aes_key = get_AES_key(mss_pri_key_file,mss_enc_aes_key_file)
            #Open IV file md_mss_key_iv.BIN.BIN
            mss_iv_file   = open(os.path.join(minidump,'md_mss_key_iv.BIN'), 'rb')
            #print "mss_iv_file: " + binascii.hexlify(mss_iv_file.read())
	        #Open MAC file md_mss_key_mac.BIN
            mss_mac_file  = open(os.path.join(minidump,'md_mss_key_mac.BIN'), 'rb')
            #print "mss_mac_file: " + binascii.hexlify(mss_mac_file.read())
            finished = False
            print "################################Decryption for APPS minidump part-1 started#############################"
            while not finished:
                line = dump_info.readline()
		        #start finding file name from here
                file_name = line.split()[0]
                print "file_name " + file_name
		        #open minidump file , find IV and MAC for minidump file
                if file_name == "md_mss_key_aes.BIN":
                    apps_iv = apps_iv_file.read(IV_SIZE)    #skipp APSS IV for md_mss_key_aes.BIN
                    apps_mac = apps_mac_file.read(MAC_SIZE) #skipp APSS MAC for md_mss_key_aes.BIN
                elif file_name == "md_mss_key_iv.BIN":
                    apps_iv = apps_iv_file.read(IV_SIZE)    #skipp APSS IV for md_mss_key_iv.BIN 
                    apps_mac = apps_mac_file.read(MAC_SIZE) #skipp APSS MAC for md_mss_key_iv.BIN
                    #break;
                elif file_name == "md_mss_key_mac.BIN":
                    apps_iv = apps_iv_file.read(IV_SIZE)    #skipp APSS IV for md_mss_key_mac.BIN
                    apps_mac = apps_mac_file.read(MAC_SIZE) #skipp APSS MAC for md_mss_key_mac.BIN
                    break;
                else :
                    enc_md_file = open(os.path.join(minidump,file_name), 'rb')
                    dec_md_file = open(os.path.join(output,file_name), 'wb')
                    apps_iv = apps_iv_file.read(IV_SIZE)
                    apps_mac = apps_mac_file.read(MAC_SIZE)
                    print "\ndecrypting..." +file_name + " file"#, "IV:" +binascii.hexlify(iv)
                    decrypt_minidump_file(enc_md_file,apps_aes_key,apps_iv,apps_mac,dec_md_file)
                    enc_md_file.close()
                    dec_md_file.close()
	
            finished = False		    
            print "\n########################Decryption for APPS minidump part-1 completed successfully########################"
            print "\n++++++++++++++++++++++++++++++Decryption for MSS minidump started...++++++++++++++++++++++++++++++++++++++"
            while not finished:
                line = dump_info.readline()
                #start finding file name from here
                file_name = line.rsplit()[0]
                #open minidump file , find IV and MAC for minidump file
                if file_name == "MD_SMEMINFO.BIN":
                    break;
                else :
                    enc_md_file = open(os.path.join(minidump,file_name), 'rb')
                    dec_md_file = open(os.path.join(output,file_name), 'wb')
                    apps_iv = apps_iv_file.read(IV_SIZE)    #skipp APSS IV for modem dump region
                    apps_mac = apps_mac_file.read(MAC_SIZE) #skipp APSS MAC for modem dump region
                    mss_iv = mss_iv_file.read(IV_SIZE)
                    mss_mac = mss_mac_file.read(MAC_SIZE/2)
                    print "\ndecrypting..." +file_name + " file"#, "IV:" +binascii.hexlify(iv)
                    decrypt_minidump_file(enc_md_file,mss_aes_key,mss_iv,mss_mac,dec_md_file)
                    enc_md_file.close()
                    dec_md_file.close()
			
            finished = False		    
            print "\n+++++++++++++++++++++++Decryption completed successfully for MSS minidump+++++++++++++++++++++++++++++++++"
            print "\n#########################Decryption for APPS minidump part-2 started...###################################"
            while not finished:
		        #start finding file name from here
                file_name = line.rsplit()[0]
                #open minidump file , find IV and MAC for minidump file
                if file_name == "md_encr_key_aes.BIN":
                    break;
                else :
                    enc_md_file = open(os.path.join(minidump,file_name), 'rb')
                    dec_md_file = open(os.path.join(output,file_name), 'wb')
                    apps_iv = apps_iv_file.read(IV_SIZE)
                    apps_mac = apps_mac_file.read(MAC_SIZE)
                    print "\ndecrypting..." +file_name + " file"#, "IV:" +binascii.hexlify(iv)
                    decrypt_minidump_file(enc_md_file,apps_aes_key,apps_iv,apps_mac,dec_md_file)
                    enc_md_file.close()
                    dec_md_file.close()
                line = dump_info.readline()
        
            print "\n#########################Decryption for APPS minidump part-2 completed successfully#######################"
    
            print "##########################################TZ Diag decryption ########################################"
            dump_info.seek(0)
            finished = False
            while not finished:
                line = dump_info.readline()
		        #start finding file name from here
                file_name = line.rsplit()[0]
                #open minidump file , find IV and MAC for minidump file
                if file_name == "md_TZ_DIAG.BIN":
                    enc_tz_diag_file = open(os.path.join(output,'md_TZ_DIAG.BIN'),'rb')
                    enc_tz_diag_file.seek(0)
                    mss_pri_key_file.seek(0)
                    #use RSA private key from devcfg
                    tz_diag_aes_key = get_TZ_Diag_AES_key(mss_pri_key_file,enc_tz_diag_file)
                    print "aes_key: " + binascii.hexlify(tz_diag_aes_key)
                    enc_tz_diag_file.seek(0x116C)
                    tz_diag_iv = enc_tz_diag_file.read(12)
                    #Reset File pointer and copy TZ diag dump until log_buf
                    enc_tz_diag_file.seek(0)
                    dec_tz_diag_file = open(os.path.join(output,'md_TZ_DIAG_decr.bin'),'wb')
                    dec_tz_diag_file.write(enc_tz_diag_file.read(0x118C))
                    #Decrypt log_buf and copy to TZ diag dump
                    enc_tz_diag_file.seek(0x118C)
                    decrypt_TZ_diag_minidump_file(enc_tz_diag_file,tz_diag_aes_key,tz_diag_iv,dec_tz_diag_file)
                    print "################################TZ Diag decryption completed successfully##################################"
                    break;
            apps_iv_file.close()
            apps_mac_file.close()
            dump_info.close()
            apps_enc_aes_key_file.close()
            mss_enc_aes_key_file.close()
            app_pri_key_file.close()
            mss_pri_key_file.close()
            mss_iv_file.close()
            mss_mac_file.close()
            enc_tz_diag_file.close()
            dec_tz_diag_file.close()
        else : # Decrypt only XBL dump
            finished = False
            print "################################Decryption for APPS minidump started#############################"
            while not finished:
                line = dump_info.readline()
		        #start finding file name from here
                file_name = line.split()[0]
                print "file_name " + file_name
		        #open minidump file , find IV and MAC for minidump file
                if file_name == "md_mss_key_aes.BIN":
                    break;
                else :
                    enc_md_file = open(os.path.join(minidump,file_name), 'rb')
                    dec_md_file = open(os.path.join(output,file_name), 'wb')
                    apps_iv = apps_iv_file.read(IV_SIZE)
                    apps_mac = apps_mac_file.read(MAC_SIZE)
                    print "\ndecrypting..." +file_name + " file"#, "IV:" +binascii.hexlify(iv)
                    decrypt_minidump_file(enc_md_file,apps_aes_key,apps_iv,apps_mac,dec_md_file)
                    enc_md_file.close()
                    dec_md_file.close()	
            print "################################Decryption for APPS minidump Completed successfully#############################" 
            apps_iv_file.close()
            apps_mac_file.close()
            dump_info.close()				
            apps_enc_aes_key_file.close()
            app_pri_key_file.close()
    
def TZ_diag_decryption (mss_priv_key, minidump) :
        #Open private key file 
        pri_key_file = open(mss_priv_key,'r')
        enc_tz_diag_file = open(os.path.join(minidump,'md_TZ_DIAG.BIN'),'rb')
            
	    #Debug : to dump encrypted AES key
	    #tz_aes_key_file = open(os.path.join(args.minidump,'aes_key.enc'),'wb')
        #enc_aes_key_file.seek(0x106C)
        #tz_aes_key_file.write(enc_aes_key_file.read(0x100))
        #tz_aes_key_file.close()
        enc_tz_diag_file = open(os.path.join(minidump,'md_TZ_DIAG.BIN'),'rb')
        enc_tz_diag_file.seek(0)
        pri_key_file.seek(0)
        #use RSA private key from devcfg
        tz_diag_aes_key = get_TZ_Diag_AES_key(pri_key_file,enc_tz_diag_file)
        print "aes_key: " + binascii.hexlify(tz_diag_aes_key)
        enc_tz_diag_file.seek(0x116C)
        tz_diag_iv = enc_tz_diag_file.read(12)
        #Reset File pointer and copy TZ diag dump until log_buf
        enc_tz_diag_file.seek(0)
        dec_tz_diag_file = open(os.path.join(minidump,'md_TZ_DIAG_decr.bin'),'wb')
        dec_tz_diag_file.write(enc_tz_diag_file.read(0x118C))
        #Decrypt log_buf and copy to TZ diag dump
        enc_tz_diag_file.seek(0x118C)
        decrypt_TZ_diag_minidump_file(enc_tz_diag_file,tz_diag_aes_key,tz_diag_iv,dec_tz_diag_file)
        print "################################TZ Diag decryption completed successfully##################################"
        enc_tz_diag_file.close()
        pri_key_file.close()
        dec_tz_diag_file.close()
    
def help():
        print "RAW minidump decrypting tool v1.1"
        print ">>>>>Help Menu<<<<<"
        print "Usage $%s -s <rawdump> <Parse_out_folder> [split raw_dump to seperate files]" % sys.argv[0]
        print "Usage $%s -d <rawdump> <Parse_out_folder> <XBL Private Key file> <Devcfg Private key file> <decrypted_output> [split raw_dump to seperate files and decrypt APSS/MSS/TZDiag]" % sys.argv[0]
        print "Usage $%s -a <rawdump> <Parse_out_folder> <XBL Private Key file> <decrypted_output> [split raw_dump to seperate files and Decrypt Apps dump]" % sys.argv[0]
        print "Usage $%s -t <Devcfg Private Key file> <encrypted TZ DIAG md_TZ_DIAG.BIN > [Decrypt md_TZ_DIAG.BIN into md_TZ_DIAG_decr.BIN ]" % sys.argv[0]
        
if __name__=='__main__':
    try:
        opts, args = getopt.gnu_getopt(sys.argv[1:], "sdat")
    except getopt.GetoptError, err:
        print str(err)
        help()
        sys.exit(-1)
    #print len(args)
    for opt, val in opts:
        if opt=="-s":
            if len(args) == 2:
                rawdump = args[0]
                out_folder = args[1]
            else:
                print "Wrong argument passed"
                help()
                sys.exit(-1)
            option = 'split'
            mdump = minidump(out_folder,0)
            mdump.split_rawdump(rawdump, option)
            break;
        elif opt == "-d" :
            if len(args) == 5:
                rawdump = args[0]
                out_folder = args[1]
                APK = args[2]
                MPK = args[3]
                output = args[4]
            else:
                print "Wrong argument passed"
                help()
                sys.exit(-1)
            option = 'split'
            mdump = minidump(out_folder,output)
            mdump.split_rawdump(rawdump, option)
            mdump.parsed_dump_decryption(APK,MPK,out_folder,output)
            break;
        elif opt == "-a" :
            if len(args) == 4:
                rawdump = args[0]
                out_folder = args[1]
                APK = args[2]
                output = args[3]
            else:
                print "Wrong argument passed"
                help()
                sys.exit(-1)
            option = 'split'
            mdump = minidump(out_folder,output)
            mdump.split_rawdump(rawdump, option)
            mdump.parsed_dump_decryption(APK,0,out_folder,output)
            break;
        elif opt == "-t" :
            if len(args) == 2:
                MPK = args[0]
                out_folder = args[1]
                if not os.path.isdir(out_folder) :
                    out_folder = "."
            else:
                print "Wrong argument passed"
                help()
                sys.exit(-1)
            TZ_diag_decryption(MPK,out_folder)
            break;
        else:
            print("wrong option")
            exit(0);
    
