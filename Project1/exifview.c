/* cs0449 - m/w
 * Project 1 - Part 2
 * Exif View Utility
 * Joshua Rodstein
 * PS#: 4021607
*/

#include <stdio.h>
#include <string.h>


/* struct stores initial 20 bytes of file */
struct jpg_header {
  unsigned short start_marker;
  unsigned short app1_marker;
  unsigned short app1_length;
  char exif_str[4];
  unsigned short null_term;
  char endianness[2];
  unsigned short version_num;
  unsigned int offset_block;
};

/* struct stores 12 byte TIFF tag */
struct tiff_tag {
  unsigned short tag_id;
  unsigned short data_type;
  unsigned int num_of_data_items;
  unsigned int offset_of_data_items;
};

/* struct stores data members for printing of final Exif Tag */
struct print_tag {
  char man[30];
  char mod[30];
  int width;
  int height;
  int iso;
  unsigned int exp[2];
  unsigned int f_stop[2];
  unsigned int len[2];
  char date[20];
};


/* Prototypes */
int parse_sub_tag(FILE*, struct print_tag);
int print_full(struct print_tag);

int main(int argc, char* argv[]) {
  short count = 0;
  int i = 0, ptr = 0;          
  FILE* fp;
  struct jpg_header jpg_head;
  struct tiff_tag t_tag;
  struct print_tag p_tag;

  /* confirm file being passed from command line
   Print 'File Not Found' error is no files passed
  */
  if ((fp = fopen(argv[1], "rb")) == NULL) {
    printf("Error: File not found\n");
    return -1;
  }

  /* read first 20 bytes of file and store in jpg_header struct
   file pointer advancces to offset 20
   if read fails print error message and returns -1
  */
  if(fread(&jpg_head, sizeof(jpg_head), 1, fp) != 0) {
    
    /* Check 4 key elements of the jpg_tag to confirm JPG Format
     if any are not matched, print corresponding error messages 
     and return -1
    */
    if(jpg_head.start_marker != 0xD8FF) {
        printf("File Format Error: Cannot Confirm JPEG Format\n");
        printf("  'Start of file marker' incorrect or not found\n");
	return -1;
    } else if (jpg_head.app1_marker != 0xE1FF) {
        printf("File Format Error: Cannot Confirm JPEG Format.\n");
	printf("  'APP1 marker' incorrect or not found\n");
	return -1;
    } else if ((strcmp(jpg_head.endianness, "II*") != 0)) {
        printf("File Format Error: Endianness Not Supported\n");
        return -1;
    } else if (strcmp(jpg_head.exif_str, "Exif") != 0) {
        printf("File Format Error: Cannot Confirm JPEG Format\n");
	printf("  'Efix tag' incorrect or or not found\n");
        return -1;
    }

  } else {
        printf("Error: Unable to Read File\n");
        return -1;
  }
  
  /* read in 2 bytes at offset 20 and store into count
   count now contains # of tags contained in the next section
  */
  fread(&count, sizeof(short), 1, fp);

  /* loop 'count' times to assure all tags are iterated over */
  for(i = 0; i < count; i++) {

    /* read in 12 byte TIFF tag and store into corresponding
     elements of a tiff_struct. Update file pointer to
     allow back tracking to read next tag.
    */ 
    fread(&t_tag, sizeof(t_tag), 1, fp);
    ptr = ftell(fp);
    
    /* if TIFF tag element is matched, seek to offset containing 
     corresponding data. read data into the corresponding element
     of the print_tag struct
    */
    if (t_tag.tag_id == 0x010F) {
      fseek(fp, (t_tag.offset_of_data_items+12), SEEK_SET);
      fread(p_tag.man, sizeof(char), t_tag.num_of_data_items, fp);
    } else if (t_tag.tag_id == 0x0110) {
      fseek(fp, (t_tag.offset_of_data_items+12), SEEK_SET);
      fread(p_tag.mod, sizeof(char), t_tag.num_of_data_items, fp);
    } else if (t_tag.tag_id == 0x8769) {
      fseek(fp, (t_tag.offset_of_data_items+12), SEEK_SET);
      /* matched tag_id containing 0x8769 means we must traverse to
       offset containing a sub_block tag. file pointer and print_tag
       struct are passed to a sub_function that will parse and store. 
      */
      parse_sub_tag(fp, p_tag);
    }
    
    /* backtrack pointer to position following the previously 
     matched tag. If tag not matched, file pointer stays the same.
    */  
    fseek(fp, ptr, SEEK_SET);
  }

    return 0;
  }

/* function recieves file pointer and a print_tag struct */
int parse_sub_tag(FILE* p, struct print_tag print){
  int i = 0, ptr = 0;
  short count = 0;
  struct tiff_tag sub_tag;

  /* again, read in # of tags and store in count*/
  fread(&count, sizeof(short), 1 , p);

  /* loop 'count' times. Read in the next tag and advance file pointer.
   each time a tag is matched, store to corresponding element of print_tag. 
   Seek to specified offset to store type 5 and type 2 data_types, then 
   store into arrays within the struct 
  */
  for(i = 0; i < count; i++) {
    fread(&sub_tag, sizeof(sub_tag), 1, p);
    /* update file pointer to allow back tracking */
    ptr = ftell(p);

    if (sub_tag.tag_id == 0xA002){
      print.width = sub_tag.offset_of_data_items;
    } else if (sub_tag.tag_id == 0xA003){
      print.height = sub_tag.offset_of_data_items;
    } else if (sub_tag.tag_id == 0x8827){
      print.iso = sub_tag.offset_of_data_items;
    } else if (sub_tag.tag_id == 0x829a){
      fseek(p, sub_tag.offset_of_data_items+12, SEEK_SET);
      fread(&print.exp, sizeof(unsigned int), 2, p);
    } else if (sub_tag.tag_id == 0x829d){
      fseek(p, sub_tag.offset_of_data_items+12, SEEK_SET);
      fread(&print.f_stop, sizeof(unsigned int), 2, p);
    } else if (sub_tag.tag_id == 0x920A){
      fseek(p, sub_tag.offset_of_data_items+12, SEEK_SET);
      fread(&print.len, sizeof(unsigned int), 2, p);
    } else if (sub_tag.tag_id == 0x9003){
      fseek(p, sub_tag.offset_of_data_items+12, SEEK_SET);
      fread(&print.date, sizeof(char), sub_tag.num_of_data_items, p);
    }
    /* reset file pointer */
    fseek(p, ptr, SEEK_SET);
  }
  
  /* call function 'print_full()' passing a print_tag struct */
  print_full(print);

  return 0;
}

/*print_full() recieves print_tag struct and prints full formated exif tag*/
int print_full(struct print_tag tag) {
    printf("Manufacturer:   %s\n", tag.man);
    printf("Model:          %s\n", tag.mod);
    printf("Exposure Time:  %d/%d second\n", tag.exp[0], tag.exp[1]);
    printf("F-Stop:         f/%g\n", (float)tag.f_stop[0]/tag.f_stop[1]);
    printf("ISO:            ISO %d\n", tag.iso);
    printf("Date Taken:     %s\n", tag.date);
    printf("Focal Length:   %g mm\n", (float)tag.len[0]/tag.len[1]);
    printf("Width:          %d pixels\n", tag.width);
    printf("Height:         %d pixels\n", tag.height);

    return 0;
}





