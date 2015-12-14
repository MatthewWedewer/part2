#include "AT89C51RC2.h"
#include <stdio.h>
#include "main.h"
#include "port.h"
#include "UART.h"
#include "SPI.h"
#include "SDCard.h"
#include "Directory_Functions.h"
#include "print_bytes.h"
#include "Long_Serial_In.h"



uint32_t idata StartofFAT;
uint16_t idata	BPB_BytesPerSec;
uint8_t  idata BPB_SecPerClus;
uint16_t idata	BPB_RsvdSecCnt;
uint8_t	idata BPB_NumFATs;
uint16_t	idata BPB_RootEntCnt;
uint16_t	idata BPB_TotSec16;
uint16_t	idata BPB_FATSz16;
uint32_t	idata BPB_HiddenSec;
uint32_t	idata BPB_TotSec32;
uint32_t	idata BPB_FATSz32;
uint32_t	idata BPB_Root_Clus;
uint32_t	idata FirstDataSec;
uint32_t	idata MBR_RelSec;
uint32_t	idata RootDirSec;
uint8_t  idata	FATtype;
uint32_t	idata FirstRootDirSec;





uint32_t read32(uint16_t offset_address, uint8_t *array_name)
{
	if (offset_address < 512)
	{
		uint32_t return_value =0;
		uint8_t temp, index;
		for (index = 0; index < 4; index++)
		{
			temp =*(array_name + offset_address + ( 3 - index));
			return_value= return_value << 8;
			return_value |= temp;
		}
		return return_value;
	}
	else
		return OFFSET_ERROR;
}

uint32_t read32_ISR(uint16_t offset_address, uint8_t *array_name)
{
	if (offset_address < 512)
	{
		uint32_t return_value =0;
		uint8_t temp, index;
		for (index = 0; index < 4; index++)
		{
			temp =*(array_name + offset_address + ( 3 - index));
			return_value= return_value << 8;
			return_value |= temp;
		}
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
		return return_value;
	}
	else
	{
		return OFFSET_ERROR;
	}
}

uint8_t mount_drive(uint8_t xdata *sector)
{
	uint8_t error_flag;
	uint16_t numofFATSectors;
	uint32_t bpb_sector, FATSz, totSec, countofClusters, DataSec;	
	
	error_flag = NO_ERRORS;
	
	read_block(0, sector);
	print_memory(sector, 512);
	if(!(read8(0, sector) == 0xEB || read8(0, sector) == 0xE9))
	{
		MBR_RelSec = read32(0x01C6, sector);
		bpb_sector = MBR_RelSec;
		printf("%-35s", "MBR_RelSec  ");
		printf("%lu", bpb_sector);
		putchar(10);
		putchar(13);
	}
	else
	{
		error_flag = WRONG_RETURN;
	}
	if(error_flag == NO_ERRORS)
	{
		read_block(bpb_sector, sector);
		print_memory(sector, 512);
		
		printf("%-20s", "BPB_BytesPerSec");
		BPB_BytesPerSec = read16(0x000B, sector);
		printf("%u", BPB_BytesPerSec);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_SecPerClus");
		BPB_SecPerClus  =  read8(0x000D, sector);
		printf("%bu", BPB_SecPerClus);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_RsvdSecCnt");
		BPB_RsvdSecCnt	= read16(0x000E, sector);
		printf("%u", BPB_RsvdSecCnt);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_NumFATs");
		BPB_NumFATs			=  read8(0x0010, sector);
		printf("%bu", BPB_NumFATs);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_RootEntCnt");
		BPB_RootEntCnt	= read16(0x0011, sector);
		printf("%u", BPB_RootEntCnt);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_TotSec16");
		BPB_TotSec16		= read16(0x0013, sector);
		printf("%u", BPB_TotSec16);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_FATSz16");
		BPB_FATSz16			= read16(0x0016, sector);
		printf("%u", BPB_FATSz16);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_HiddenSec");
		BPB_HiddenSec		= read32(0x001C, sector);
		printf("%lu", BPB_HiddenSec);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_TotSec32");
		BPB_TotSec32		= read32(0x0020, sector);
		printf("%lu", BPB_TotSec32);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "BPB_FATSz32");
		BPB_FATSz32			= read32(0x0024, sector);
		printf("%lu", BPB_FATSz32);
		putchar(10);
		putchar(13);

		printf("%-20s", "BPB_Root_Clu");
		BPB_Root_Clus		= read32(0x002C, sector);
		printf("%lu", BPB_Root_Clus);
		putchar(10);
		putchar(13);
		
		printf("%-20s", "RootDirSec");
		RootDirSec = ((BPB_RootEntCnt * 32) + (BPB_BytesPerSec - 1)) / BPB_BytesPerSec;
		printf("%lu", RootDirSec);
		putchar(10);
		putchar(13);
		
		
		
		if(BPB_FATSz16 == 0)
		{
			FATSz = BPB_FATSz32;
			printf("%-20s", "FAT size32");
			printf("%lu", FATSz);
			putchar(10);
			putchar(13);
		}
		else
		{
			FATSz = BPB_FATSz16;
			error_flag = WRONG_FAT_TYPE;
			printf("%lu", "error wrong fattype ");
			putchar(10);
			putchar(13);
		}
			
		if(BPB_TotSec16 == 0)
		{
			totSec = BPB_TotSec32;
			printf("%-20s", "TotSec32");
			printf("%lu", totSec);
			putchar(10);
			putchar(13);
		}
		else
		{
			totSec = BPB_TotSec16;
			error_flag = WRONG_FAT_TYPE;
		}
		
		numofFATSectors = FATSz * BPB_NumFATs;  // Dont think this is used.
		printf("%-20s", "numofFATSectors");
		printf("%lu", numofFATSectors);
		putchar(10);
		putchar(13);
		
		DataSec = totSec - (BPB_RsvdSecCnt + (BPB_NumFATs * FATSz) + RootDirSec);
		printf("%-20s", "DataSec");
		printf("%lu", DataSec);
		putchar(10);
		putchar(13);
		
		countofClusters = DataSec / BPB_SecPerClus;
		printf("%-20s", "countofClusters");
		printf("%lu", countofClusters);
		putchar(10);
		putchar(13);
		
		FirstDataSec = BPB_RsvdSecCnt + (BPB_NumFATs * FATSz) + RootDirSec + MBR_RelSec;
		printf("%-20s", "FirstDataSec");
		printf("%lu", FirstDataSec);
		putchar(10);
		putchar(13);
		
		FirstRootDirSec = ((BPB_Root_Clus - 2) * BPB_SecPerClus) + FirstDataSec;
		printf("%-20s", "FirstRootDirSec");
		printf("%lu", FirstRootDirSec);
		putchar(10);
		putchar(13);		
		
		
		if(countofClusters < 4085) 
		{
			FATtype = FAT12;
			error_flag = WRONG_FAT_TYPE;
			FATtype = FAT16;
			error_flag = WRONG_FAT_TYPE;
			printf("%-20s", "FAT size 12");
			putchar(10);
			putchar(13);
		}
		else if (countofClusters < 65525)
		{	
			FATtype = FAT16;
			error_flag = WRONG_FAT_TYPE;
			printf("%-20s", "FAT size 16");
			printf("%lu", FATtype);
			putchar(10);
			putchar(13);
		}
		else 
		{
			FATtype = FAT32;
			printf("%-20s", "FAT size 32");
			putchar(10);
			putchar(13);
		}
	}
	
	return error_flag;
}
	

uint32_t first_sector(uint32_t cluster_num)
{
	uint32_t first_sector;
	if (cluster_num == 0)
	{
		return FirstRootDirSec;
	}
	else
	{
		first_sector=( (cluster_num - 2) * BPB_SecPerClus)+ FirstDataSec;
		printf("%-20s", "first_sector");
		printf("%8.8lX", first_sector);
		putchar(10);
		putchar(13);
			
		printf("%-20s", "cluster_num");
		printf("%8.8lX", cluster_num);
		putchar(10);
		putchar(13);
			
		printf("%-20s", "BPB_SecPerClus");
		printf("%8.8bX", BPB_SecPerClus);
		putchar(10);
		putchar(13);
			
		printf("%-20s", "FirstDataSec");
		printf("%8.8lX", FirstDataSec);
		putchar(10);
		putchar(13);
		
		return first_sector;
	}	
}


uint32_t first_sector_ISR(uint32_t cluster_num)
{
	uint32_t first_sector;
	if (cluster_num == 0)
	{
		return FirstRootDirSec;
	}
	else
	{
		first_sector=( (cluster_num - 2) * BPB_SecPerClus)+ FirstDataSec;
		//printf("%-20s", "first_sector");
		//printf("%8.8lX", first_sector);
		//putchar(10);
		//putchar(13);
			
		//printf("%-20s", "cluster_num");
		//printf("%8.8lX", cluster_num);
		//putchar(10);
		//putchar(13);
			
		//printf("%-20s", "BPB_SecPerClus");
		//printf("%8.8bX", BPB_SecPerClus);
		//putchar(10);
		//putchar(13);
			
//		printf("%-20s", "FirstDataSec");
//		printf("%8.8lX", FirstDataSec);
//		putchar(10);
//		putchar(13);
		
		return first_sector;
	}	
}


uint32_t find_next_cluster(uint32_t Current_Cluster, uint8_t xdata * array_name)
{
	uint32_t FATOffset, ThisFATEntOffset, Next_Cluster, ThisFATSecNum;
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




uint32_t find_next_cluster_ISR(uint32_t Current_Cluster, uint8_t xdata * array_name)
{
	uint32_t FATOffset, ThisFATEntOffset, Next_Cluster, ThisFATSecNum;
	StartofFAT = BPB_RsvdSecCnt + MBR_RelSec;
	FATOffset = Current_Cluster * FAT32; // where FATtype = 4 for FAT32 or FATtype = 2 for FAT16 
	ThisFATSecNum = (FATOffset/BPB_BytesPerSec) + StartofFAT; //Where 
	//Step 2) Read the FAT sector where the cluster entry is located into XRAM 
	ncs =0; 
	send_command_ISR(17,ThisFATSecNum); 
	read_block_ISR(512,array_name); 
	ncs=1; 
	//Step 3) Determine the offset where the cluster entry is located 
	//ThisFATEntOffset = REM(FATOffset/BPB_BytsPerSec) or 
	ThisFATEntOffset = (FATOffset % BPB_BytesPerSec); 
	//Step 4) Read the next cluster from the entry at this offset and return this value 
	Next_Cluster = (read32_ISR(ThisFATEntOffset, array_name)) & 0x0FFFFFFF; 
	return Next_Cluster;
}


//uint8_t Open_File(uint32_t Cluster, uint8_t xdata * array_in)
//{ uint8_t error_flag, index, input;
//	uint32_t this_cluster, next_sector;
//	index = 0;
//	this_cluster = Cluster;
//	next_sector = first_sector(Cluster);
//	printf("%-20s", "next_sector");
//	printf("%8.8lX", next_sector);
//	putchar(10);
//	putchar(13);
//	error_flag = read_block(next_sector, array_in);
//	if (error_flag == NO_ERRORS)
//	{
//		print_memory(array_in, BPB_BytesPerSec);
//		printf("%-35s", "Continue? ( y= '1', n= '2' )");
//		input = long_serial_input();
//		while(input == 1 && error_flag == NO_ERRORS)
//		{
//			index++;
//			if(index > BPB_SecPerClus)
//			{
//				index = 0;
//				this_cluster = find_next_cluster(this_cluster, array_in);
//				next_sector = first_sector(this_cluster);
//			}
//			error_flag = read_block((next_sector + (BPB_BytesPerSec * index)), array_in);
//			print_memory(array_in, BPB_BytesPerSec);
//			printf("%-35s", "Continue? ( y= '1', n= '2' )");
//			input = long_serial_input();
//		}
//	}
//	
//	return error_flag;
//}


/***********************************************************************
DESC: Prints all short file name entries for a given directory 
INPUT: Starting Sector of the directory and the pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint16_t number of entries found in the directory
CAUTION: Supports FAT16, SD_shift must be set before using this function
************************************************************************/



uint16_t  Print_Directory(uint32_t Sector_num, uint8_t xdata *array_in)
{ 
   uint32_t Sector, max_sectors;
   uint16_t i, entries;
   uint8_t temp8, j, attr, out_val, error_flag;
   uint8_t *values;

   values=array_in;
   entries=0;
   i=0;
   if (Sector_num<FirstDataSec)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSec;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=BPB_SecPerClus;
   }
   Sector=Sector_num;
   error_flag=read_block(Sector, values);
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
              error_flag=read_block(Sector, values);
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
  uint8_t *values;
  values = array_in;
  entries = 0;
  i = 0;
  return_clus = 0;
  if (Sector_num < FirstDataSec)  // included for FAT16 compatibility
  { 
     max_sectors = RootDirSec;   // maximum sectors in a FAT16 root directory
  }
  else
  {
     max_sectors = BPB_SecPerClus;
  }
  Sector = Sector_num;
  error_flag=read_block(Sector, values);
  if(error_flag == NO_ERRORS)
  {
    do
    {
      temp8 = read8(0 + i, values);  // read first byte to see if empty
      if((temp8 != 0xE5) && (temp8 != 0x00))
	    {  
	      attr = read8(0x0b + i, values);
		    if((attr & 0x0E) == 0)    // if hidden do not print
		    {
		      entries++;
          if(entries == Entry)
          {
						if(FATtype == FAT32)
            {
							return_clus=read8(21 + i, values);
							return_clus &= 0x0F;            // makes sure upper four bits are clear
							return_clus = return_clus<<8;
              return_clus |= read8(20 + i, values);
              return_clus = return_clus << 8;
            }
            return_clus |= read8(27 + i, values);
						return_clus = return_clus << 8;
            return_clus |= read8(26 + i, values);
						attr=read8(0x0b + i, values);
						if(attr & 0x10) return_clus|=directory_bit;
              temp8 = 0;    // forces a function exit
          }      
				}
      }
			i = i + 32;  // next entry
			if(i > 510)
			{
		    Sector++;
				if((Sector-Sector_num) < max_sectors)
				{
					error_flag = read_block(Sector, values);
					if(error_flag != NO_ERRORS)
					{
						return_clus = no_entry_found;
            temp8 = 0; 
					}
					i = 0;
				}
				else
				{
					temp8 = 0;                       // forces a function exit
				}
			}    
		}while(temp8 != 0);
  }
  else
  {
		return_clus=no_entry_found;
  }
  if(return_clus == 0)
		return_clus=no_entry_found;
  return return_clus;
}





