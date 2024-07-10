typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned int ssize_t;

/* The DVDinput handle, add stuff here for new input methods. */
typedef struct {
  /* libdvdcss handle */
  void* dvdcss;
  
  /* dummy file input */
  int fd;
} dvd_input_t;

typedef struct {
    /* Basic information. */
    int isImageFile;
  
    /* Hack for keeping track of the css status. 
     * 0: no css, 1: perhaps (need init of keys), 2: have done init */
    int css_state;
    int css_title; /* Last title that we have called dvdinpute_title for. */

    /* Information required for an image file. */
    dvd_input_t dev;

    /* Information required for a directory path drive. */
    char *path_root;
  
    /* Filesystem cache */
    int udfcache_level; /* 0 - turned off, 1 - on */
    void *udfcache;
} dvd_reader_t;

typedef struct {
    /* Basic information. */
    dvd_reader_t *dvd;
  
    /* Hack for selecting the right css title. */
    int css_title;

    /* Information required for an image file. */
    uint32_t lb_start;
    uint32_t seek_pos;

    /* Information required for a directory path drive. */
    size_t title_sizes[ 9 ];
    dvd_input_t title_devs[ 9 ];

    /* Calculated at open-time, size in blocks. */
    ssize_t filesize;
} dvd_file_t;

/**
 * Video Attributes.
 */
typedef struct {
#ifdef WORDS_BIGENDIAN
  unsigned char mpeg_version         : 2;
  unsigned char video_format         : 2;
  unsigned char display_aspect_ratio : 2;
  unsigned char permitted_df         : 2;
  
  unsigned char line21_cc_1          : 1;
  unsigned char line21_cc_2          : 1;
  unsigned char unknown1             : 1;
  unsigned char bit_rate             : 1;
  
  unsigned char picture_size         : 2;
  unsigned char letterboxed          : 1;
  unsigned char film_mode            : 1;
#else
  unsigned char permitted_df         : 2;
  unsigned char display_aspect_ratio : 2;
  unsigned char video_format         : 2;
  unsigned char mpeg_version         : 2;
  
  unsigned char film_mode            : 1;
  unsigned char letterboxed          : 1;
  unsigned char picture_size         : 2;
  
  unsigned char bit_rate             : 1;
  unsigned char unknown1             : 1;
  unsigned char line21_cc_2          : 1;
  unsigned char line21_cc_1          : 1;
#endif
} video_attr_t;

/**
 * Audio Attributes.
 */
typedef struct {
#ifdef WORDS_BIGENDIAN
  unsigned char audio_format           : 3;
  unsigned char multichannel_extension : 1;
  unsigned char lang_type              : 2;
  unsigned char application_mode       : 2;
  
  unsigned char quantization           : 2;
  unsigned char sample_frequency       : 2;
  unsigned char unknown1               : 1;
  unsigned char channels               : 3;
#else
  unsigned char application_mode       : 2;
  unsigned char lang_type              : 2;
  unsigned char multichannel_extension : 1;
  unsigned char audio_format           : 3;
  
  unsigned char channels               : 3;
  unsigned char unknown1               : 1;
  unsigned char sample_frequency       : 2;
  unsigned char quantization           : 2;
#endif
  uint16_t lang_code;
  uint8_t  lang_extension;
  uint8_t  code_extension;
  uint8_t unknown3;
  union {
    struct {
#ifdef WORDS_BIGENDIAN
      unsigned char unknown4           : 1;
      unsigned char channel_assignment : 3;
      unsigned char version            : 2;
      unsigned char mc_intro           : 1; /* probably 0: true, 1:false */
      unsigned char mode               : 1; /* Karaoke mode 0: solo 1: duet */
#else
      unsigned char mode               : 1;
      unsigned char mc_intro           : 1;
      unsigned char version            : 2;
      unsigned char channel_assignment : 3;
      unsigned char unknown4           : 1;
#endif
    } karaoke;
    struct {
#ifdef WORDS_BIGENDIAN
      unsigned char unknown5           : 4;
      unsigned char dolby_encoded      : 1; /* suitable for surround decoding */
      unsigned char unknown6           : 3;
#else
      unsigned char unknown6           : 3;
      unsigned char dolby_encoded      : 1;
      unsigned char unknown5           : 4;
#endif
    } surround;
  } app_info;
} audio_attr_t;

/**
 * Subpicture Attributes.
 */
typedef struct {
  /*
   * type: 0 not specified
   *       1 language
   *       2 other
   * coding mode: 0 run length
   *              1 extended
   *              2 other
   * language: indicates language if type == 1
   * lang extension: if type == 1 contains the lang extension
   */
#ifdef WORDS_BIGENDIAN
  unsigned char code_mode : 3;
  unsigned char zero1     : 3;
  unsigned char type      : 2;
#else
  unsigned char type      : 2;
  unsigned char zero1     : 3;
  unsigned char code_mode : 3;
#endif
  uint8_t  zero2;
  uint16_t lang_code;
  uint8_t  lang_extension;
  uint8_t  code_extension;
} subp_attr_t;

/**
 * Video Manager Information Management Table.
 */
typedef struct {
  char     vmg_identifier[12];
  uint32_t vmg_last_sector;
  uint8_t  zero_1[12];
  uint32_t vmgi_last_sector;
  uint8_t  zero_2;
  uint8_t  specification_version;
  uint32_t vmg_category;
  uint16_t vmg_nr_of_volumes;
  uint16_t vmg_this_volume_nr;
  uint8_t  disc_side;
  uint8_t  zero_3[19];
  uint16_t vmg_nr_of_title_sets;  /* Number of VTSs. */
  char     provider_identifier[32];
  uint64_t vmg_pos_code;
  uint8_t  zero_4[24];
  uint32_t vmgi_last_byte;
  uint32_t first_play_pgc;
  uint8_t  zero_5[56];
  uint32_t vmgm_vobs;             /* sector */
  uint32_t tt_srpt;               /* sector */
  uint32_t vmgm_pgci_ut;          /* sector */
  uint32_t ptl_mait;              /* sector */
  uint32_t vts_atrt;              /* sector */
  uint32_t txtdt_mgi;             /* sector */
  uint32_t vmgm_c_adt;            /* sector */
  uint32_t vmgm_vobu_admap;       /* sector */
  uint8_t  zero_6[32];
  
  video_attr_t vmgm_video_attr;
  uint8_t  zero_7;
  uint8_t  nr_of_vmgm_audio_streams; /* should be 0 or 1 */
  audio_attr_t vmgm_audio_attr;
  audio_attr_t zero_8[7];
  uint8_t  zero_9[17];
  uint8_t  nr_of_vmgm_subp_streams; /* should be 0 or 1 */
  subp_attr_t  vmgm_subp_attr;
  subp_attr_t  zero_10[27];  /* XXX: how much 'padding' here? */
} vmgi_mat_t;

/**
 * User Operations.
 */
typedef struct {
#ifdef WORDS_BIGENDIAN
  unsigned int zero                           : 7; /* 25-31 */
  unsigned int video_pres_mode_change         : 1; /* 24 */
  
  unsigned int karaoke_audio_pres_mode_change : 1; /* 23 */
  unsigned int angle_change                   : 1;
  unsigned int subpic_stream_change           : 1;
  unsigned int audio_stream_change            : 1;
  unsigned int pause_on                       : 1;
  unsigned int still_off                      : 1;
  unsigned int button_select_or_activate      : 1;
  unsigned int resume                         : 1; /* 16 */
  
  unsigned int chapter_menu_call              : 1; /* 15 */
  unsigned int angle_menu_call                : 1;
  unsigned int audio_menu_call                : 1;
  unsigned int subpic_menu_call               : 1;
  unsigned int root_menu_call                 : 1;
  unsigned int title_menu_call                : 1;
  unsigned int backward_scan                  : 1;
  unsigned int forward_scan                   : 1; /* 8 */
  
  unsigned int next_pg_search                 : 1; /* 7 */
  unsigned int prev_or_top_pg_search          : 1;
  unsigned int time_or_chapter_search         : 1;
  unsigned int go_up                          : 1;
  unsigned int stop                           : 1;
  unsigned int title_play                     : 1;
  unsigned int chapter_search_or_play         : 1;
  unsigned int title_or_time_play             : 1; /* 0 */
#else
  unsigned int video_pres_mode_change         : 1; /* 24 */
  unsigned int zero                           : 7; /* 25-31 */
  
  unsigned int resume                         : 1; /* 16 */
  unsigned int button_select_or_activate      : 1;
  unsigned int still_off                      : 1;
  unsigned int pause_on                       : 1;
  unsigned int audio_stream_change            : 1;
  unsigned int subpic_stream_change           : 1;
  unsigned int angle_change                   : 1;
  unsigned int karaoke_audio_pres_mode_change : 1; /* 23 */
  
  unsigned int forward_scan                   : 1; /* 8 */
  unsigned int backward_scan                  : 1;
  unsigned int title_menu_call                : 1;
  unsigned int root_menu_call                 : 1;
  unsigned int subpic_menu_call               : 1;
  unsigned int audio_menu_call                : 1;
  unsigned int angle_menu_call                : 1;
  unsigned int chapter_menu_call              : 1; /* 15 */
  
  unsigned int title_or_time_play             : 1; /* 0 */
  unsigned int chapter_search_or_play         : 1;
  unsigned int title_play                     : 1;
  unsigned int stop                           : 1;
  unsigned int go_up                          : 1;
  unsigned int time_or_chapter_search         : 1;
  unsigned int prev_or_top_pg_search          : 1;
  unsigned int next_pg_search                 : 1; /* 7 */
#endif
} user_ops_t;

/**
 * DVD Time Information.
 */
typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t frame_u; /* The two high bits are the frame rate. */
} dvd_time_t;

typedef struct {
#ifdef WORDS_BIGENDIAN
  unsigned int block_mode       : 2;
  unsigned int block_type       : 2;
  unsigned int seamless_play    : 1;
  unsigned int interleaved      : 1;
  unsigned int stc_discontinuity: 1;
  unsigned int seamless_angle   : 1;
  
  unsigned int playback_mode    : 1;  /**< When set, enter StillMode after each VOBU */
  unsigned int restricted       : 1;  /**< ?? drop out of fastforward? */
  unsigned int unknown2         : 6;
#else
  unsigned char seamless_angle   : 1;
  unsigned char stc_discontinuity: 1;
  unsigned char interleaved      : 1;
  unsigned char seamless_play    : 1;
  unsigned char block_type       : 2;
  unsigned char block_mode       : 2;
  
  unsigned char unknown2         : 6;
  unsigned char restricted       : 1;
  unsigned char playback_mode    : 1;
#endif
  uint8_t still_time;
  uint8_t cell_cmd_nr;
  dvd_time_t playback_time;
  uint32_t first_sector;
  uint32_t first_ilvu_end_sector;
  uint32_t last_vobu_start_sector;
  uint32_t last_sector;
} cell_playback_t;

typedef struct {
  uint16_t zero_1;
  uint8_t  nr_of_programs;
  uint8_t  nr_of_cells;
  dvd_time_t playback_time;
  user_ops_t prohibited_ops;
  uint16_t audio_control[8]; /* New type? */
  uint32_t subp_control[32]; /* New type? */
  uint16_t next_pgc_nr;
  uint16_t prev_pgc_nr;
  uint16_t goup_pgc_nr;
  uint8_t  still_time;
  uint8_t  pg_playback_mode;
  uint32_t palette[16]; /* New type struct {zero_1, Y, Cr, Cb} ? */
  uint16_t command_tbl_offset;
  uint16_t program_map_offset;
  uint16_t cell_playback_offset;
  uint16_t cell_position_offset;
  void *command_tbl;
  void  *program_map;
  cell_playback_t *cell_playback;
  void *cell_position;
} pgc_t;
#define PGC_SIZE 236

typedef struct {
#ifdef WORDS_BIGENDIAN
  unsigned int zero1      : 7;
  unsigned int ach0_gme   : 1;

  unsigned int zero2      : 7;
  unsigned int ach1_gme   : 1;

  unsigned int zero3      : 4;
  unsigned int ach2_gv1e  : 1;
  unsigned int ach2_gv2e  : 1;
  unsigned int ach2_gm1e  : 1;
  unsigned int ach2_gm2e  : 1;

  unsigned int zero4      : 4;
  unsigned int ach3_gv1e  : 1;
  unsigned int ach3_gv2e  : 1;
  unsigned int ach3_gmAe  : 1;
  unsigned int ach3_se2e  : 1;

  unsigned int zero5      : 4;
  unsigned int ach4_gv1e  : 1;
  unsigned int ach4_gv2e  : 1;
  unsigned int ach4_gmBe  : 1;
  unsigned int ach4_seBe  : 1;
#else
  unsigned char ach0_gme   : 1;
  unsigned char zero1      : 7;

  unsigned char ach1_gme   : 1;
  unsigned char zero2      : 7;

  unsigned char ach2_gm2e  : 1;
  unsigned char ach2_gm1e  : 1;
  unsigned char ach2_gv2e  : 1;
  unsigned char ach2_gv1e  : 1;
  unsigned char zero3      : 4;

  unsigned char ach3_se2e  : 1;
  unsigned char ach3_gmAe  : 1;
  unsigned char ach3_gv2e  : 1;
  unsigned char ach3_gv1e  : 1;
  unsigned char zero4      : 4;

  unsigned char ach4_seBe  : 1;
  unsigned char ach4_gmBe  : 1;
  unsigned char ach4_gv2e  : 1;
  unsigned char ach4_gv1e  : 1;
  unsigned char zero5      : 4;
#endif
  uint8_t zero6[19];
} multichannel_ext_t;

typedef struct {
  char vts_identifier[12];
  uint32_t vts_last_sector;
  uint8_t  zero_1[12];
  uint32_t vtsi_last_sector;
  uint8_t  zero_2;
  uint8_t  specification_version;
  uint32_t vts_category;
  uint16_t zero_3;
  uint16_t zero_4;
  uint8_t  zero_5;
  uint8_t  zero_6[19];
  uint16_t zero_7;
  uint8_t  zero_8[32];
  uint64_t zero_9;
  uint8_t  zero_10[24];
  uint32_t vtsi_last_byte;
  uint32_t zero_11;
  uint8_t  zero_12[56];
  uint32_t vtsm_vobs;       /* sector */
  uint32_t vtstt_vobs;      /* sector */
  uint32_t vts_ptt_srpt;    /* sector */
  uint32_t vts_pgcit;       /* sector */
  uint32_t vtsm_pgci_ut;    /* sector */
  uint32_t vts_tmapt;       /* sector */
  uint32_t vtsm_c_adt;      /* sector */
  uint32_t vtsm_vobu_admap; /* sector */
  uint32_t vts_c_adt;       /* sector */
  uint32_t vts_vobu_admap;  /* sector */
  uint8_t  zero_13[24];
  
  video_attr_t vtsm_video_attr;
  uint8_t  zero_14;
  uint8_t  nr_of_vtsm_audio_streams; /* should be 0 or 1 */
  audio_attr_t vtsm_audio_attr;
  audio_attr_t zero_15[7];
  uint8_t  zero_16[17];
  uint8_t  nr_of_vtsm_subp_streams; /* should be 0 or 1 */
  subp_attr_t vtsm_subp_attr;
  subp_attr_t zero_17[27];
  uint8_t  zero_18[2];
  
  video_attr_t vts_video_attr;
  uint8_t  zero_19;
  uint8_t  nr_of_vts_audio_streams;
  audio_attr_t vts_audio_attr[8];
  uint8_t  zero_20[17];
  uint8_t  nr_of_vts_subp_streams;
  subp_attr_t vts_subp_attr[32];
  uint16_t zero_21;
  multichannel_ext_t vts_mu_audio_attr[8];
  /* XXX: how much 'padding' here, if any? */
} vtsi_mat_t;

/**
 * The following structure defines an IFO file.  The structure is divided into
 * two parts, the VMGI, or Video Manager Information, which is read from the
 * VIDEO_TS.[IFO,BUP] file, and the VTSI, or Video Title Set Information, which
 * is read in from the VTS_XX_0.[IFO,BUP] files.
 */
typedef struct {
  dvd_file_t *file;
  
  /* VMGI */
  vmgi_mat_t     *vmgi_mat;
  void      *tt_srpt;
  void          *first_play_pgc;    
  void     *ptl_mait;
  void     *vts_atrt;
  void    *txtdt_mgi;
  
  /* Common */
  void      *pgci_ut;
  void		*menu_c_adt;
  void		*menu_vobu_admap;
  
  /* VTSI */
  vtsi_mat_t	*vtsi_mat;
  void		*vts_ptt_srpt;
  pgc_t		*vts_pgcit;
  void		*vts_tmapt;
  void		*vts_c_adt;
  void		*vts_vobu_admap;
} ifo_handle_t;

typedef dvd_reader_t* (*DVDOpen)( const char *ppath );
typedef ifo_handle_t* (*ifoOpen)(dvd_reader_t *dvd, int title);
typedef void (*ifoClose)(ifo_handle_t *ifofile);
