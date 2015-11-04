#include "AT89C51RC2.h"
#include <stdio.h>
#include "main.h"
#include "port.h"
#include "UART.h"
#include "SPI.h"
#include "SDCard.h"
#include "Directory_Functions.h"
#include "print_bytes.h"
//#include "File_System.h"
#include "read_sector.h"

uint32_t idata FirstDataSec_g, StartofFAT_g, FirstRootDirSec_g, RootDirSecs_g;
uint16_t idata BytesPerSec_g;
uint8_t idata SecPerClus_g, FATtype_g, BytesPerSecShift_g,FATshift_g;

	extern uint16_t xdata	BPB_BytesPerSec;
	extern uint8_t  xdata BPB_SecPerClus;
	extern uint16_t xdata	BPB_RsvdSecCnt;
	extern uint8_t	xdata BPB_NumFATs;
	extern uint16_t	xdata BPB_RootEntCnt;
	extern uint16_t	xdata BPB_TotSec16;
	extern uint16_t	xdata BPB_FATSz16;
	extern uint32_t	xdata BPB_HiddenSec;
	extern uint32_t	xdata BPB_TotSec32;
	extern uint32_t	xdata BPB_FATSz32;
	extern uint32_t	xdata BPB_Root_Clus;
	extern uint32_t	xdata FirstDataSec;
	extern uint32_t	xdata MBR_RelSec;
	extern uint8_t	xdata RootDirSec;

	




uint32_t read32(uint16_t offset_address, uint8_t *array_name)
{
	if (offset_address <512)
	{
		uint32_t return_value =0;
		uint8_t temp, index;
				for (index = 0; index < 4; index++)
		{
		temp =*(array_name + offset_address + ( 3 - index));
			return_value= return_value << 8;
			return_value |= temp;
		}
		printf("%lu",return_value);
		return return_value;
	}
	else
		return OFFSET_ERROR;
}

uint16_t read16(uint16_t offset_address, uint8_t *array_name)
{
	uint16_t return_value =0;
	uint8_t temp, index;
	if(offset_address < 512)
	{
		for (index = 0; index < 2; index++)
		{
			temp =*(array_name + offset_address + ( 1 - index));
			return_value= return_value << 8;
			return_value |= temp;
		}
		printf("%u",return_value);
		return return_value;
	}
	else
	{
		return OFFSET_ERROR;
	}
}

uint8_t read8(uint16_t offset_address, uint8_t *array_name)
{
	uint8_t return_value =0;
	uint8_t temp;
	if(offset_address < 512)
	{
		temp =*(array_name + offset_address);
		return_value= return_value << 8;
		return_value |= temp;
		printf("%bu",return_value);
		return return_value;
	}
	else
	{
		return OFFSET_ERROR;
	}
}

uint8_t mount_drive(void)
{
	uint8_t sector[512];
	uint8_t error_flag; //RootDirSec;
	uint16_t numofFATSectors;
	uint32_t bpb_sector, FATSz, totSec, FATtype, countofClusters, FirstDataSec;
	
	error_flag = NO_ERRORS;
	
	read_sector(0, 512, sector);
	if(!(read8(0, sector) == 0xEB || read8(0, sector) == 0xE9))
	{
		MBR_RelSec = read32(0x01C6, sector);
	}
	else
	{
		error_flag = WRONG_RETURN;
	}
	if(error_flag == NO_ERRORS)
	{
		read_sector(bpb_sector, 512, sector);
		
		BPB_BytesPerSec = read16(0x000B, sector);
		BPB_SecPerClus  =  read8(0x000D, sector);
		BPB_RsvdSecCnt	= read16(0x000E, sector);
		BPB_NumFATs			=  read8(0x0010, sector);
		BPB_RootEntCnt	= read16(0x0011, sector);
		BPB_TotSec16		= read16(0x0013, sector);
		BPB_FATSz16			= read16(0x0016, sector);
		BPB_HiddenSec		= read32(0x001C, sector);
		BPB_TotSec32		= read32(0x0020, sector);
		BPB_FATSz32			= read32(0x0024, sector);
		BPB_Root_Clus		= read32(0x002C, sector);
		
		
		RootDirSec = ((BPB_RootEntCnt * 32) + (BPB_BytesPerSec - 1)) / BPB_BytesPerSec;
		
		if(BPB_FATSz16 == 0)
		{
			FATSz = BPB_FATSz32;
			printf("%-20s", "FAT size32 is ");
			printf("%8.8bX", FATSz);
		}
		else
		{
			FATSz = BPB_FATSz16;
			error_flag = WRONG_FAT_TYPE;
		}
			
		if(BPB_TotSec16 == 0)
		{
			totSec = BPB_TotSec32;
			printf("%-20s", "TotSec32 is ");
			printf("%8.8bX", totSec);
		}
		else
		{
			totSec = BPB_TotSec16;
			error_flag = WRONG_FAT_TYPE;
		}
		numofFATSectors = FATSz * BPB_NumFATs;  // Dont think this is used.
		printf("%-20s", "numofFATSectors is ");
		printf("%8.8bX", numofFATSectors);
		FirstDataSec = totSec - (BPB_RsvdSecCnt + (BPB_NumFATs * FATSz) + RootDirSec);
		printf("%-20s", "FirstDataSec is ");
		printf("%8.8bX", FirstDataSec);
		countofClusters = FirstDataSec / BPB_SecPerClus;
		printf("%-20s", "countofClusters is ");
		printf("%8.8bX", countofClusters);
		if(countofClusters < 4085) 
		{
			FATtype = FAT12;
			error_flag = WRONG_FAT_TYPE;
		}
		else if (countofClusters < 65525)
		{	
			FATtype = FAT16;
			error_flag = WRONG_FAT_TYPE;
		}
		else 
		{
			FATtype = FAT32;
		}
		
		printf("%-12s", "FAT size is ");
		printf("%2d", FATtype);
	}
	
	return error_flag;
}
	

uint32_t first_sector(uint32_t cluster_num)
{
	uint32_t first_sector;
	if (cluster_num == 0)
		{
		return RootDirSec;
		}
	else
		{
		first_sector=((cluster_num - 2)*BPB_SecPerClus)+ FirstDataSec;
			return first_sector;
		}	
}



uint32_t find_next_clus(uint32_t Current_Cluster, uint8_t xdata * array_name)
{
	uint32_t FATOffset, ThisFATEntOffset, Next_Cluster, ThisFATSecNum, StartofFAT;
	StartofFAT = BPB_RsvdSecCnt + MBR_RelSec;
	FATOffset = Current_Cluster * FAT32; // where FATtype = 4 for FAT32 or FATtype = 2 for FAT16 
		ThisFATSecNum = (FATOffset/BPB_BytesPerSec) + StartofFAT; //Where 
	//Step 2) Read the FAT sector where the cluster entry is located into XRAM 
	ncs =0; 
	send_command(17,ThisFATSecNum); 
	read_block(512,array_name); 
	ncs=1; 
	//Step 3) Determine the offset where the cluster entry is located 
	//ThisFATEntOffset = REM(FATOffset/BPB_BytsPerSec) or 
		ThisFATEntOffset = (FATOffset % BPB_BytesPerSec); 
	//Step 4) Read the next cluster from the entry at this offset and return this value 
	 Next_Cluster = (read32(ThisFATEntOffset, array_name)) & 0x0FFFFFFF; 
	return Next_Cluster;
}





/***********************************************************************
DESC: Prints all short file name entries for a given directory 
INPUT: Starting Sector of the directory and the pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint16_t number of entries found in the directory
CAUTION: Supports FAT16, SD_shift must be set before using this function
************************************************************************/



uint16_t  Print_Directory(uint32_t Sector_num, uint8_t xdata * array_in)
{ 
   uint32_t Sector, max_sectors;
   uint16_t i, entries;
   uint8_t temp8, j, attr, out_val, error_flag;
   uint8_t * values;

   values=array_in;
   entries=0;
   i=0;
   if (Sector_num<FirstDataSec_g)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSecs_g;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=SecPerClus_g;
   }
   Sector=Sector_num;
   error_flag=read_sector(Sector, BytesPerSec_g, values);
   if(error_flag==NO_ERRORS)
   {
     do
     {
 
	    temp8=read8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read8(0x0b+i,values);
		   	LED2=1; // should be yellow led
		   if((attr&0x0E)==0)   // if hidden, system or Vol_ID bit is set do not print
		   {
		      entries++;
			  printf("%5d. ",entries);  // print entry number with a fixed width specifier
		      for(j=0;j<8;j++)
			  {
			     out_val=read8(i+j,values);   // print the 8 byte name
			     putchar(out_val);
			  }
              if((attr&0x10)==0x10)  // indicates directory
			  {
			     for(j=8;j<11;j++)
			     {
			        out_val=read8(i+j,values);
			        putchar(out_val);
			     }
			     printf("[DIR]\n");
			  }
			  else       // print a period and the three byte extension for a file
			  {
			     putchar(0x2E);       
			     for(j=8;j<11;j++)
			     {
			        out_val=read8(i+j,values);
			        putchar(out_val);
			     }
			     putchar(0x0d);
                 putchar(0x0a);
			  }
		    }

		}
		i=i+32;  // next entry

		if(i>510)
		{
		  Sector++;
          if((Sector-Sector_num)<max_sectors)
		  {
              error_flag=read_sector(Sector, BytesPerSec_g, values);
			  if(error_flag!=NO_ERRORS)
			    {
			      entries=0;   // no entries found indicates disk read error
				  temp8=0;     // forces a function exit
			    }
			    i=0;
		  }
		  else
		  {
			  entries=entries|more_entries;  // set msb to indicate more entries in another cluster
			  temp8=0;                       // forces a function exit
		  }
		}
       
	  }while(temp8!=0);
	}
	else
	{
	   entries=0;    // no entries found indicates disk read error
	}
    return entries;
 }


/***********************************************************************
DESC: Uses the same method as Print_Directory to locate short file names,
      but locates a specified entry and returns and cluster  
INPUT: Starting Sector of the directory, an entry number and a pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint32_t with cluster in lower 28 bits.  Bit 28 set if this is 
         a directory entry, clear for a file.  Bit 31 set for error.
CAUTION: 
************************************************************************/

uint32_t Read_Dir_Entry(uint32_t Sector_num, uint16_t Entry, uint8_t xdata * array_in)
{ 
   uint32_t Sector, max_sectors, return_clus;
   uint16_t i, entries;
   uint8_t temp8, attr, error_flag;
   uint8_t * values;

   values=array_in;
   entries=0;
   i=0;
   return_clus=0;
   if (Sector_num<FirstDataSec_g)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSecs_g;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=SecPerClus_g;
   }
   Sector=Sector_num;
   error_flag=read_sector(Sector, BytesPerSec_g, values);
   if(error_flag==NO_ERRORS)
   {
     do
     {
        temp8=read8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read8(0x0b+i,values);
		   if((attr&0x0E)==0)    // if hidden do not print
		   {
		      entries++;
              if(entries==Entry)
              {
			    if(FATtype_g==FAT32)
                {
                   return_clus=read8(21+i,values);
				   return_clus&=0x0F;            // makes sure upper four bits are clear
				   return_clus=return_clus<<8;
                   return_clus|=read8(20+i,values);
                   return_clus=return_clus<<8;
                }
                return_clus|=read8(27+i,values);
			    return_clus=return_clus<<8;
                return_clus|=read8(26+i,values);
			    attr=read8(0x0b+i,values);
			    if(attr&0x10) return_clus|=directory_bit;
                temp8=0;    // forces a function exit
              }
              
		   }
        }
		i=i+32;  // next entry
		if(i>510)
		{
		   Sector++;
		   if((Sector-Sector_num)<max_sectors)
		   {
              error_flag=read_sector(Sector, BytesPerSec_g, values);
			  if(error_flag!=NO_ERRORS)
			  {
			     return_clus=no_entry_found;
                 temp8=0; 
			  }
			  i=0;
		   }
		   else
		   {
			  temp8=0;                       // forces a function exit
		   }
		}
        
	 }while(temp8!=0);
   }
   else
   {
	 return_clus=no_entry_found;
   }
   if(return_clus==0) return_clus=no_entry_found;
   return return_clus;
}





