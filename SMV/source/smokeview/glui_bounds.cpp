// $Date$ 
// $Revision$
// $Author$

#define CPP
#include "options.h"

// svn revision character string
extern "C" char glui_bounds_revision[];
char glui_bounds_revision[]="$Revision$";

#include <stdio.h>
#include <string.h>
#include GLUT_H
#include <math.h>

#include "smokeviewvars.h"

int nevacloaded, nplot3dloaded, nsmoke3dloaded, nisoloaded, nsliceloaded, nvsliceloaded, npartloaded, npatchloaded;

extern "C" void set_memcheck(int index);
extern "C" void colorbar_global2local(void);
extern "C" void PLOT3D_CB(int var);
extern "C" void FileShow_CB(int var);

#ifdef pp_MEMDEBUG
void Memcheck_CB(int val);
#endif
void SETslicemax(int setslicemax, float slicemax,int setslicechopmax, float slicechopmax);
void SETslicemin(int setslicemin, float slicemin, int setslicechopmin, float slicechopmin);
void Bounds_DLG_CB(int var);
void PART_CB(int var);
void Bound_CB(int var);
void Iso_CB(int var);
void Smoke3D_CB(int var);
void Time_CB(int var);
void Script_CB(int var);
void boundmenu(GLUI_Rollout **ROLLOUT_bound, GLUI_Rollout **ROLLOUT_chop, GLUI_Panel *PANEL_panel, char *button_title,
          GLUI_EditText **EDIT_con_min,GLUI_EditText **EDIT_con_max,
          GLUI_RadioGroup **RADIO_con_setmin,GLUI_RadioGroup **RADIO_con_setmax,
          GLUI_Checkbox **CHECKBOX_con_setchopmin, GLUI_Checkbox **CHECKBOX_con_setchopmax,
          GLUI_EditText **EDIT_con_chopmin, GLUI_EditText **EDIT_con_chopmax,
          GLUI_StaticText **STATIC_con_min_unit,GLUI_StaticText **STATIC_con_max_unit,
          GLUI_StaticText **STATIC_con_cmin_unit,GLUI_StaticText **STATIC_con_cmax_unit,
          GLUI_Button **BUTTON_update, GLUI_Button **BUTTON_reload,
          int *setminval, int *setmaxval,
          float *minval, float *maxval,
          int *setchopminval, int *setchopmaxval,
          float *chopminval, float *chopmaxval,
          int updatebounds,
          int truncatebounds,
          GLUI_Update_CB FILE_CB);

GLUI_Rollout *ROLLOUT_slice_bound=NULL;
GLUI_Rollout *ROLLOUT_slice_chop=NULL;
GLUI_Rollout *ROLLOUT_part_bound=NULL;
GLUI_Rollout *ROLLOUT_part_chop=NULL;
GLUI_Rollout *ROLLOUT_zone_bound=NULL;

#ifdef pp_MEMDEBUG
#define MEMCHECK 1
#endif

#define SMOOTH_SURFACES 402
#define SORT_SURFACES 401
#define ISO_SURFACE 1
#define ISO_OUTLINE 2
#define ISO_POINTS 3
#define SETVALMIN 1
#define SETVALMAX 2
#define VALMIN 3
#define VALMAX 4
#define FILETYPEINDEX 5
#define FILEUPDATE 6
#define FILERELOAD 7
#define FILEUPDATEDATA 8
#define UPDATEPLOT 10
#define PLOTISO 11
#define SHOWCHAR 12
#define CHOPVALMIN 13
#define CHOPVALMAX 14
#define SETCHOPMINVAL 15
#define SETCHOPMAXVAL 16
#define CHOPUPDATE 17
#define FRAMELOADING 18
#define STREAKLENGTH 19
#define TRACERS 21
#define PLOTISOTYPE 22
#define CACHE_BOUNDARYDATA 23
#define SHOWPATCH_BOTH 24
#define HIDEPATCHSURFACE 25
#define DATA_transparent 26
#define UNLOAD_QDATA 203
#define SET_TIME 204
#define TBOUNDS 205
#define TBOUNDS_USE 206
#define RELOAD_DATA 207
#define SHOW_EVAC_SLICES 208
#define DATA_EVAC_COLORING 209
#define SLICE_VECTORSKIP 210
#define PLOT3D_VECTORSKIP 211

#define SCRIPT_START 31
#define SCRIPT_STOP 32
#define SCRIPT_LIST 33
#define SCRIPT_SAVEINI 34
#define SCRIPT_EDIT_INI 35
#define SCRIPT_SETSUFFIX 36
#define SCRIPT_RUNSCRIPT 37
#define SCRIPT_LOADINI 38
#define SCRIPT_RENDER 41
#define SCRIPT_RENDER_SUFFIX 42
#define SCRIPT_RENDER_DIR 43
#define SCRIPT_STEP_NOW 44
#define SCRIPT_CANCEL_NOW 45

#define ZONEVALMIN 50
#define ZONEVALMAX 51
#define SETZONEVALMIN 52
#define SETZONEVALMAX 53

#define SAVE_SETTINGS 99
#define CLOSE 98
#define COMPRESS_FILES 97
#define OVERWRITE 96
#define COMPRESS_AUTOLOADED 91
#define ERASE 95
#define STARTUP 94
#define SAVE_FILE_LIST 93
#define LOAD_FILES 92
#define COLORBAR_EXTREME2 109
#define TRANSPARENTLEVEL 110
#define COLORBAR_LIST2 112
#define COLORBAR_SMOOTH 113
#define RESEARCH_MODE 114
#define COLORBAND 115

#define UPDATE_VECTOR 101
#define UPDATE_VECTOR_FROM_SMV 102

#define TRUNCATE_BOUNDS 1
#define DONT_TRUNCATE_BOUNDS 0
#define UPDATE_BOUNDS 1
#define RELOAD_BOUNDS 0
#define UPDATERELOAD_BOUNDS 2

#define LINE_CONTOUR_VALUE 301
#define UPDATE_LINE_CONTOUR_VALUE 302

#define FILESHOW_particle    10
#define FILESHOW_slice       11
#define FILESHOW_vslice      12
#define FILESHOW_boundary    13
#define FILESHOW_3dsmoke     14
#define FILESHOW_isosurface  15
#define FILESHOW_evac 19
#define FILESHOW_plot3d 16


GLUI *glui_bounds=NULL;

GLUI_Button *BUTTON_updatebound=NULL;
GLUI_Button *BUTTON_reloadbound=NULL;
GLUI_Button *BUTTON_compress=NULL;
GLUI_Button *BUTTON_step=NULL;
GLUI_Button *BUTTON_script_stop=NULL;
GLUI_Button *BUTTON_script_start=NULL;
GLUI_Button *BUTTON_script_saveini=NULL;
GLUI_Button *BUTTON_script_render=NULL;
GLUI_Button *BUTTON_update_line_contour=NULL;
GLUI_Button *BUTTON_ini_load=NULL;
GLUI_Button *BUTTON_script_setsuffix=NULL;
GLUI_Button *BUTTON_script_runscript=NULL;
GLUI_Button *BUTTON_RELOAD=NULL;
GLUI_Button *BUTTON_SETTIME=NULL;
GLUI_Button *BUTTON_EVAC = NULL;
GLUI_Button *BUTTON_PART = NULL;
GLUI_Button *BUTTON_SLICE = NULL;
GLUI_Button *BUTTON_VSLICE = NULL;
GLUI_Button *BUTTON_PLOT3D = NULL;
GLUI_Button *BUTTON_3DSMOKE = NULL;
GLUI_Button *BUTTON_BOUNDARY = NULL;
GLUI_Button *BUTTON_ISO = NULL;

#ifdef pp_MEMDEBUG
GLUI_Rollout *PANEL_memcheck=NULL;
#endif
GLUI_Rollout *ROLLOUT_script=NULL;
GLUI_Rollout *ROLLOUT_config = NULL;
GLUI_Rollout *ROLLOUT_boundary = NULL;
GLUI_Rollout *ROLLOUT_autoload=NULL;
GLUI_Rollout *ROLLOUT_compress=NULL;
GLUI_Rollout *ROLLOUT_plot3d=NULL,*ROLLOUT_evac=NULL,*ROLLOUT_part=NULL,*ROLLOUT_slice=NULL,*ROLLOUT_bound=NULL,*ROLLOUT_iso=NULL;
GLUI_Rollout *ROLLOUT_smoke3d=NULL,*ROLLOUT_volsmoke3d=NULL;
GLUI_Rollout *ROLLOUT_time=NULL,*ROLLOUT_colorbar=NULL;
GLUI_Rollout *ROLLOUT_outputpatchdata=NULL;
GLUI_Rollout *ROLLOUT_filebounds = NULL;
GLUI_Rollout *ROLLOUT_showhide = NULL;

GLUI_Panel *PANEL_files = NULL;
GLUI_Panel *PANEL_bounds = NULL;
GLUI_Panel *PANEL_slice_average = NULL;
GLUI_Panel *PANEL_zone_a=NULL, *PANEL_zone_b=NULL;
GLUI_Panel *PANEL_evac_direction=NULL;
GLUI_Panel *PANEL_pan1=NULL;
GLUI_Panel *PANEL_pan2=NULL;
GLUI_Panel *PANEL_pan3=NULL;
GLUI_Panel *PANEL_isosurface=NULL;
GLUI_Panel *PANEL_vector=NULL;
GLUI_Panel *PANEL_slice_vector=NULL;
GLUI_Panel *PANEL_line_contour=NULL;
GLUI_Panel *PANEL_run=NULL;
GLUI_Panel *PANEL_record=NULL;
GLUI_Panel *PANEL_script1=NULL;
GLUI_Panel *PANEL_script1a=NULL;
GLUI_Panel *PANEL_script1b=NULL;
GLUI_Panel *PANEL_script1c=NULL;
GLUI_Panel *PANEL_script2a=NULL;
GLUI_Panel *PANEL_script2b=NULL;
GLUI_Panel *PANEL_script3=NULL;
GLUI_Panel *PANEL_transparency2=NULL;
GLUI_Panel *PANEL_time2=NULL;
GLUI_Panel *PANEL_time1a=NULL;
GLUI_Panel *PANEL_time2a=NULL;
GLUI_Panel *PANEL_time2b=NULL;
GLUI_Panel *PANEL_time2c=NULL;
GLUI_Panel *PANEL_outputpatchdata=NULL;

GLUI_Spinner *SPINNER_labels_transparency_data2=NULL;
GLUI_Spinner *SPINNER_transparent_level=NULL;
GLUI_Spinner *SPINNER_line_contour_num=NULL;
GLUI_Spinner *SPINNER_line_contour_width=NULL;
GLUI_Spinner *SPINNER_line_contour_min=NULL;
GLUI_Spinner *SPINNER_line_contour_max=NULL;
GLUI_Spinner *SPINNER_timebounds=NULL;
GLUI_Spinner *SPINNER_tload_begin=NULL;
GLUI_Spinner *SPINNER_tload_end=NULL;
GLUI_Spinner *SPINNER_tload_skip=NULL;
GLUI_Spinner *SPINNER_plot3d_vectorpointsize=NULL,*SPINNER_plot3d_vectorlinewidth=NULL,*SPINNER_plot3d_vectorlinelength=NULL;
GLUI_Spinner *SPINNER_sliceaverage=NULL;
GLUI_Spinner *SPINNER_partpointstep=NULL;
GLUI_Spinner *SPINNER_smoke3dzipstep=NULL;
GLUI_Spinner *SPINNER_slicezipstep=NULL;
GLUI_Spinner *SPINNER_isozipstep=NULL;
GLUI_Spinner *SPINNER_boundzipstep=NULL;
GLUI_Spinner *SPINNER_partstreaklength=NULL;
GLUI_Spinner *SPINNER_partpointsize=NULL;
GLUI_Spinner *SPINNER_isopointsize=NULL;
GLUI_Spinner *SPINNER_isolinewidth=NULL;
GLUI_Spinner *SPINNER_plot3dpointsize=NULL;
GLUI_Spinner *SPINNER_plot3dlinewidth=NULL;
GLUI_Spinner *SPINNER_streaklinewidth=NULL;
GLUI_Spinner *SPINNER_vectorpointsize=NULL;
GLUI_Spinner *SPINNER_vectorlinewidth=NULL;
GLUI_Spinner *SPINNER_vectorlinelength=NULL;
GLUI_Spinner *SPINNER_slicevectorskip=NULL;
GLUI_Spinner *SPINNER_plot3dvectorskip=NULL;

GLUI_Listbox *LIST_scriptlist=NULL;
GLUI_Listbox *LIST_ini_list=NULL;

GLUI_EditText *EDIT_zone_min=NULL, *EDIT_zone_max=NULL;
GLUI_EditText *EDIT_ini=NULL;
GLUI_EditText *EDIT_renderdir=NULL;
GLUI_EditText *EDIT_rendersuffix=NULL;
GLUI_EditText *EDIT_slice_min=NULL, *EDIT_slice_max=NULL;
GLUI_EditText *EDIT_slice_chopmin=NULL, *EDIT_slice_chopmax=NULL;
GLUI_EditText *EDIT_patch_chopmin=NULL, *EDIT_patch_chopmax=NULL;
GLUI_EditText *EDIT_part_chopmin=NULL, *EDIT_part_chopmax=NULL;
GLUI_EditText *EDIT_patch_min=NULL, *EDIT_patch_max=NULL;
GLUI_EditText *EDIT_part_min=NULL, *EDIT_part_max=NULL;
GLUI_EditText *EDIT_p3_min=NULL, *EDIT_p3_max=NULL;
GLUI_EditText *EDIT_p3_chopmin=NULL, *EDIT_p3_chopmax=NULL;

GLUI_Checkbox *CHECKBOX_show_slices_and_vectors=NULL;
GLUI_Checkbox *CHECKBOX_cache_boundarydata=NULL;
GLUI_Checkbox *CHECKBOX_showpatch_both=NULL;
GLUI_Checkbox *CHECKBOX_showchar=NULL, *CHECKBOX_showonlychar;
GLUI_Checkbox *CHECKBOX_showtrisurface=NULL;
GLUI_Checkbox *CHECKBOX_showtrioutline=NULL;
GLUI_Checkbox *CHECKBOX_showtripoints=NULL;
GLUI_Checkbox *CHECKBOX_defer=NULL;
GLUI_Checkbox *CHECKBOX_script_step=NULL;
GLUI_Checkbox *CHECKBOX_show_evac_slices=NULL;
GLUI_Checkbox *CHECKBOX_constant_coloring=NULL;
GLUI_Checkbox *CHECKBOX_show_evac_color=NULL;
GLUI_Checkbox *CHECKBOX_data_coloring=NULL;
GLUI_Checkbox *CHECKBOX_transparentflag2=NULL;
GLUI_Checkbox *CHECKBOX_sort2=NULL;
GLUI_Checkbox *CHECKBOX_smooth2=NULL;
GLUI_Checkbox *CHECKBOX_overwrite_all=NULL;
GLUI_Checkbox *CHECKBOX_compress_autoloaded=NULL;
GLUI_Checkbox *CHECKBOX_erase_all=NULL;
GLUI_Checkbox *CHECKBOX_multi_task=NULL;
GLUI_Checkbox *CHECKBOX_slice_setchopmin=NULL;
GLUI_Checkbox *CHECKBOX_slice_setchopmax=NULL;
GLUI_Checkbox *CHECKBOX_p3_setchopmin=NULL, *CHECKBOX_p3_setchopmax=NULL;
GLUI_Checkbox *CHECKBOX_patch_setchopmin=NULL, *CHECKBOX_patch_setchopmax=NULL;
GLUI_Checkbox *CHECKBOX_part_setchopmin=NULL, *CHECKBOX_part_setchopmax=NULL;
GLUI_Checkbox *CHECKBOX_showtracer=NULL;
GLUI_Checkbox *CHECKBOX_cellcenter_slice_interp=NULL;
GLUI_Checkbox *CHECKBOX_skip_subslice=NULL;
GLUI_Checkbox *CHECKBOX_turb_slice=NULL;
GLUI_Checkbox *CHECKBOX_average_slice=NULL;
GLUI_Checkbox *CHECKBOX_cache_qdata=NULL;
GLUI_Checkbox *CHECKBOX_use_tload_begin=NULL;
GLUI_Checkbox *CHECKBOX_use_tload_end=NULL;
GLUI_Checkbox *CHECKBOX_use_tload_skip=NULL;
GLUI_Checkbox *CHECKBOX_research_mode=NULL;


GLUI_RadioGroup *RADIO_showhide = NULL;
GLUI_RadioGroup *RADIO_contour_type = NULL;
GLUI_RadioGroup *RADIO_zone_setmin=NULL, *RADIO_zone_setmax=NULL;
GLUI_RadioGroup *RADIO_bf=NULL, *RADIO_p3=NULL,*RADIO_slice=NULL;
GLUI_RadioGroup *RADIO_part5=NULL;
GLUI_RadioGroup *RADIO_plot3d_isotype=NULL;
GLUI_RadioGroup *RADIO_plot3d_display=NULL;
GLUI_RadioGroup *RADIO_slice_setmin=NULL, *RADIO_slice_setmax=NULL;
GLUI_RadioGroup *RADIO_patch_setmin=NULL, *RADIO_patch_setmax=NULL;
GLUI_RadioGroup *RADIO_part_setmin=NULL, *RADIO_part_setmax=NULL;
#ifdef pp_MEMDEBUG
GLUI_RadioGroup *RADIO_memcheck=NULL;
#endif
GLUI_RadioGroup *RADIO_p3_setmin=NULL, *RADIO_p3_setmax=NULL;

GLUI_RadioButton *RADIOBUTTON_plot3d_iso_hidden=NULL;
GLUI_RadioButton *RADIOBUTTON_zone_permin=NULL;
GLUI_RadioButton *RADIOBUTTON_zone_permax=NULL;

GLUI_StaticText *STATIC_bound_min_unit=NULL;
GLUI_StaticText *STATIC_bound_max_unit=NULL;
GLUI_StaticText *STATIC_slice_min_unit=NULL;
GLUI_StaticText *STATIC_slice_max_unit=NULL;
GLUI_StaticText *STATIC_part_min_unit=NULL;
GLUI_StaticText *STATIC_part_max_unit=NULL;
GLUI_StaticText *STATIC_plot3d_min_unit=NULL;
GLUI_StaticText *STATIC_plot3d_max_unit=NULL;
GLUI_StaticText *STATIC_bound_cmin_unit=NULL;
GLUI_StaticText *STATIC_bound_cmax_unit=NULL;
GLUI_StaticText *STATIC_slice_cmin_unit=NULL;
GLUI_StaticText *STATIC_slice_cmax_unit=NULL;
GLUI_StaticText *STATIC_part_cmin_unit=NULL;
GLUI_StaticText *STATIC_part_cmax_unit=NULL;
GLUI_StaticText *STATIC_plot3d_cmin_unit=NULL;
GLUI_StaticText *STATIC_plot3d_cmax_unit=NULL;

#define ZONE_ROLLOUT 0
#define SMOKE3D_ROLLOUT 1
#define BOUNDARY_ROLLOUT 2
#define ISO_ROLLOUT 3
#define PART_ROLLOUT 4
#define EVAC_ROLLOUT 5
#define PLOT3D_ROLLOUT 6
#define SLICE_ROLLOUT 7

#define LOAD_ROLLOUT 0
#define SHOWHIDE_ROLLOUT 1
#define COMPRESS_ROLLOUT 2
#define SCRIPT_ROLLOUT 3
#define CONFIG_ROLLOUT 4
#define FILEBOUNDS_ROLLOUT 5
#define TIME_ROLLOUT 6

procdata boundprocinfo[8], fileprocinfo[7];
int nboundprocinfo = 0, nfileprocinfo = 0;

/* ------------------ Bound_Rollout_CB ------------------------ */

void Bound_Rollout_CB(int var){
  toggle_rollout(boundprocinfo, nboundprocinfo, var);
}

/* ------------------ File_Rollout_CB ------------------------ */

void File_Rollout_CB(int var){
  toggle_rollout(fileprocinfo, nfileprocinfo, var);
}

/* ------------------ update_glui_zonebounds ------------------------ */

extern "C" void update_glui_zonebounds(void){
  if(EDIT_zone_min!=NULL)EDIT_zone_min->set_float_val(zonemin);
  if(EDIT_zone_max!=NULL)EDIT_zone_max->set_float_val(zonemax);
}
/* ------------------ update_glui_vecfactor ------------------------ */

extern "C" void update_glui_vecfactor(void){
  if(SPINNER_plot3d_vectorlinelength!=NULL)SPINNER_plot3d_vectorlinelength->set_float_val(vecfactor);
  if(SPINNER_vectorlinelength!=NULL)SPINNER_vectorlinelength->set_float_val(vecfactor);
}

/* ------------------ update_glui_part_units ------------------------ */

extern "C" void update_glui_part_units(void){
  if(STATIC_part_min_unit!=NULL){
    if(partmin_unit!=NULL){
      STATIC_part_min_unit->set_name((char *)partmin_unit);
    }
    else{
      STATIC_part_min_unit->set_name((char *)"");
    }
  }
  if(STATIC_part_cmin_unit!=NULL){
    if(partmin_unit!=NULL){
      STATIC_part_cmin_unit->set_name((char *)partmin_unit);
    }
    else{
      STATIC_part_cmin_unit->set_name((char *)"");
    }
  }
  if(STATIC_part_max_unit!=NULL){
    if(partmax_unit!=NULL){
      STATIC_part_max_unit->set_name((char *)partmax_unit);
    }
    else{
      STATIC_part_max_unit->set_name((char *)"");
    }
  }
  if(STATIC_part_cmax_unit!=NULL){
    if(partmax_unit!=NULL){
      STATIC_part_cmax_unit->set_name((char *)partmax_unit);
    }
    else{
      STATIC_part_cmax_unit->set_name((char *)"");
    }
  }
}

/* ------------------ update_glui_plot3d_units ------------------------ */

extern "C" void update_glui_plot3d_units(void){
  if(STATIC_plot3d_min_unit!=NULL&&plot3dmin_unit!=NULL){
    STATIC_plot3d_min_unit->set_name((char *)plot3dmin_unit);
  }
  if(STATIC_plot3d_max_unit!=NULL&&plot3dmax_unit!=NULL){
    STATIC_plot3d_max_unit->set_name((char *)plot3dmax_unit);
  }
  if(STATIC_plot3d_cmin_unit!=NULL&&plot3dmin_unit!=NULL){
    STATIC_plot3d_cmin_unit->set_name((char *)plot3dmin_unit);
  }
  if(STATIC_plot3d_cmax_unit!=NULL&&plot3dmax_unit!=NULL){
    STATIC_plot3d_cmax_unit->set_name((char *)plot3dmax_unit);
  }
}

/* ------------------ update_glui_slice_units ------------------------ */

extern "C" void update_glui_slice_units(void){
  if(STATIC_slice_min_unit!=NULL&&slicemin_unit!=NULL){
    STATIC_slice_min_unit->set_name((char *)slicemin_unit);
  }
  if(STATIC_slice_max_unit!=NULL&&slicemax_unit!=NULL){
    STATIC_slice_max_unit->set_name((char *)slicemax_unit);
  }
  if(STATIC_slice_cmin_unit!=NULL&&slicemin_unit!=NULL){
    STATIC_slice_cmin_unit->set_name((char *)slicemin_unit);
  }
  if(STATIC_slice_cmax_unit!=NULL&&slicemax_unit!=NULL){
    STATIC_slice_cmax_unit->set_name((char *)slicemax_unit);
  }
}

/* ------------------ update_glui_patch_units ------------------------ */

extern "C" void update_glui_patch_units(void){
  if(STATIC_bound_min_unit!=NULL&&patchmin_unit!=NULL){
    STATIC_bound_min_unit->set_name((char *)patchmin_unit);
  }
  if(STATIC_bound_max_unit!=NULL&&patchmax_unit!=NULL){
    STATIC_bound_max_unit->set_name((char *)patchmax_unit);
  }
  if(STATIC_bound_cmin_unit!=NULL&&patchmin_unit!=NULL){
    STATIC_bound_cmin_unit->set_name((char *)patchmin_unit);
  }
  if(STATIC_bound_cmax_unit!=NULL&&patchmax_unit!=NULL){
    STATIC_bound_cmax_unit->set_name((char *)patchmax_unit);
  }
}

/* ------------------ update_research_mode ------------------------ */

extern "C" void update_research_mode(void){
  Slice_CB(RESEARCH_MODE);
  if(CHECKBOX_research_mode!=NULL)CHECKBOX_research_mode->set_int_val(research_mode);
}
/* ------------------ update_script_stop ------------------------ */

extern "C" void update_script_stop(void){
  if(BUTTON_script_start!=NULL)BUTTON_script_start->enable();
  if(BUTTON_script_stop!=NULL)BUTTON_script_stop->disable();
  if(BUTTON_script_runscript!=NULL)BUTTON_script_runscript->enable();
  if(EDIT_renderdir!=NULL)EDIT_renderdir->enable();
}

/* ------------------ update_script_start ------------------------ */

extern "C" void update_script_start(void){
  if(BUTTON_script_start!=NULL)BUTTON_script_start->disable();
  if(BUTTON_script_stop!=NULL)BUTTON_script_stop->enable();
  if(BUTTON_script_runscript!=NULL)BUTTON_script_runscript->disable();
  if(EDIT_renderdir!=NULL)EDIT_renderdir->disable();
}

/* ------------------ update_defer ------------------------ */

extern "C" void update_defer(void){
  CHECKBOX_defer->set_int_val(defer_file_loading);
}

/* ------------------ update_script_step ------------------------ */

extern "C" void update_script_step(void){
  CHECKBOX_script_step->set_int_val(script_step);
  if(script_step==1){
    BUTTON_step->enable();
  }
  else{
    BUTTON_step->disable();
  }
}

/* ------------------ update_evac_parms ------------------------ */

extern "C" void update_evac_parms(void){
  if(CHECKBOX_show_evac_slices!=NULL)CHECKBOX_show_evac_slices->set_int_val(show_evac_slices);
  if(CHECKBOX_constant_coloring!=NULL)CHECKBOX_constant_coloring->set_int_val(constant_evac_coloring);
  if(CHECKBOX_data_coloring!=NULL)CHECKBOX_data_coloring->set_int_val(data_evac_coloring);
  if(CHECKBOX_show_evac_color!=NULL)CHECKBOX_show_evac_color->set_int_val(show_evac_colorbar);
}

/* ------------------ update_glui_plot3d ------------------------ */

extern "C" void update_glui_plot3d(void){
  PLOT3D_CB(UNLOAD_QDATA);
}

/* ------------------ PART_CB_INIT ------------------------ */

extern "C" void PART_CB_INIT(void){
  PART_CB(FILETYPEINDEX);
}

/* ------------------ glui_bounds_setup ------------------------ */

extern "C" void glui_bounds_setup(int main_window){  
  int i;
  int nradio;

  update_glui_bounds=0;
  if(glui_bounds!=NULL){
    glui_bounds->close();
    glui_bounds=NULL;
  }
  overwrite_all=0;
  glui_bounds = GLUI_Master.create_glui( "Files/Bounds",0,0,0 );
  if(showbounds_dialog==0)glui_bounds->hide();

  PANEL_files = glui_bounds->add_panel("Files", true);

  ROLLOUT_autoload = glui_bounds->add_rollout_to_panel(PANEL_files,_("Auto load"), false, LOAD_ROLLOUT, File_Rollout_CB);
  ADDPROCINFO(fileprocinfo, nfileprocinfo, ROLLOUT_autoload, LOAD_ROLLOUT);
  
  glui_bounds->add_checkbox_to_panel(ROLLOUT_autoload, _("Auto load at startup"),
    &loadfiles_at_startup, STARTUP, Bound_CB);
  glui_bounds->add_button_to_panel(ROLLOUT_autoload, _("Save auto load file list"), SAVE_FILE_LIST, Bound_CB);
  glui_bounds->add_button_to_panel(ROLLOUT_autoload, _("Auto load now"), LOAD_FILES, Bound_CB);

  // -------------- Show/Hide Loaded files -------------------

  if(npartinfo > 0 || nsliceinfo > 0 || nvsliceinfo > 0 || nisoinfo > 0 || npatchinfo || nsmoke3dinfo > 0 || nplot3dinfo > 0){
    ROLLOUT_showhide = glui_bounds->add_rollout_to_panel(PANEL_files,"Show/Hide", false, SHOWHIDE_ROLLOUT, File_Rollout_CB);
    ADDPROCINFO(fileprocinfo, nfileprocinfo, ROLLOUT_showhide, SHOWHIDE_ROLLOUT);
  
    RADIO_showhide = glui_bounds->add_radiogroup_to_panel(ROLLOUT_showhide, &showhide_option);
    glui_bounds->add_radiobutton_to_group(RADIO_showhide, _("Show"));
    glui_bounds->add_radiobutton_to_group(RADIO_showhide, _("Show Only"));
    glui_bounds->add_radiobutton_to_group(RADIO_showhide, _("Hide"));

    glui_bounds->add_column_to_panel(ROLLOUT_showhide, false);

    if(nevac > 0){}
    if(npartinfo > 0 && nevac != npartinfo)BUTTON_PART = glui_bounds->add_button_to_panel(ROLLOUT_showhide, "Particle", FILESHOW_particle, FileShow_CB);
    if(nevac > 0)BUTTON_EVAC = glui_bounds->add_button_to_panel(ROLLOUT_showhide, "Evacuation", FILESHOW_evac, FileShow_CB);
    if(nsliceinfo > 0)BUTTON_SLICE = glui_bounds->add_button_to_panel(ROLLOUT_showhide, "Slice", FILESHOW_slice, FileShow_CB);
    if(nvsliceinfo > 0)BUTTON_VSLICE = glui_bounds->add_button_to_panel(ROLLOUT_showhide, "Vector", FILESHOW_vslice, FileShow_CB);
    if(nisoinfo > 0)BUTTON_ISO = glui_bounds->add_button_to_panel(ROLLOUT_showhide, "Isosurface", FILESHOW_isosurface, FileShow_CB);
    if(npatchinfo > 0)BUTTON_BOUNDARY = glui_bounds->add_button_to_panel(ROLLOUT_showhide, "Boundary", FILESHOW_boundary, FileShow_CB);
    if(nsmoke3dinfo > 0)BUTTON_3DSMOKE = glui_bounds->add_button_to_panel(ROLLOUT_showhide, "3D smoke", FILESHOW_3dsmoke, FileShow_CB);
    if(nplot3dinfo > 0)BUTTON_PLOT3D = glui_bounds->add_button_to_panel(ROLLOUT_showhide, "Plot3D", FILESHOW_plot3d, FileShow_CB);

    update_showhidebuttons();
  }


#ifdef pp_COMPRESS
  if(smokezippath != NULL && (npatchinfo > 0 || nsmoke3dinfo > 0 || nsliceinfo > 0)){
    ROLLOUT_compress = glui_bounds->add_rollout_to_panel(PANEL_files,_("Compress"), false, COMPRESS_ROLLOUT, File_Rollout_CB);
    ADDPROCINFO(fileprocinfo, nfileprocinfo, ROLLOUT_compress, COMPRESS_ROLLOUT);
  
    CHECKBOX_erase_all = glui_bounds->add_checkbox_to_panel(ROLLOUT_compress, _("Erase compressed files"),
      &erase_all, ERASE, Bound_CB);
    CHECKBOX_overwrite_all = glui_bounds->add_checkbox_to_panel(ROLLOUT_compress, _("Overwrite compressed files"),
      &overwrite_all, OVERWRITE, Bound_CB);
    CHECKBOX_compress_autoloaded = glui_bounds->add_checkbox_to_panel(ROLLOUT_compress, _("Compress only autoloaded files"),
      &compress_autoloaded, COMPRESS_AUTOLOADED, Bound_CB);
    if(nsliceinfo > 0){
      SPINNER_slicezipstep = glui_bounds->add_spinner_to_panel(ROLLOUT_compress, _("Slice frame Skip"), GLUI_SPINNER_INT, &slicezipskip,
        FRAMELOADING, Slice_CB);
      SPINNER_slicezipstep->set_int_limits(0, 100);
    }
    if(nisoinfo > 0){
      SPINNER_isozipstep = glui_bounds->add_spinner_to_panel(ROLLOUT_compress, _("Compressed file frame skip"), GLUI_SPINNER_INT, &isozipskip,
        FRAMELOADING, Iso_CB);
      SPINNER_isozipstep->set_int_limits(0, 100);
    }
    if(nsmoke3dinfo > 0){
      SPINNER_smoke3dzipstep = glui_bounds->add_spinner_to_panel(ROLLOUT_compress, _("3D smoke frame skip"), GLUI_SPINNER_INT, &smoke3dzipskip,
        FRAMELOADING, Smoke3D_CB);
      SPINNER_smoke3dzipstep->set_int_limits(0, 100);
    }
    if(npatchinfo > 0){
      SPINNER_boundzipstep = glui_bounds->add_spinner_to_panel(ROLLOUT_compress, _("Boundary file frame skip"),
        GLUI_SPINNER_INT, &boundzipskip, FRAMELOADING, Bound_CB);
      SPINNER_boundzipstep->set_int_limits(0, 100);
    }
    BUTTON_compress = glui_bounds->add_button_to_panel(ROLLOUT_compress, _("Run smokezip"), COMPRESS_FILES, Bound_CB);
  }
#endif

  ROLLOUT_script = glui_bounds->add_rollout_to_panel(PANEL_files,"Scripts", false, SCRIPT_ROLLOUT, File_Rollout_CB);
  ADDPROCINFO(fileprocinfo, nfileprocinfo, ROLLOUT_script, SCRIPT_ROLLOUT);

  PANEL_script1 = glui_bounds->add_panel_to_panel(ROLLOUT_script, _("Script files"), false);
  PANEL_record = glui_bounds->add_panel_to_panel(PANEL_script1, _("Record"), true);

  PANEL_script1a = glui_bounds->add_panel_to_panel(PANEL_record, "", false);
  BUTTON_script_start = glui_bounds->add_button_to_panel(PANEL_script1a, _("Start"), SCRIPT_START, Script_CB);
  glui_bounds->add_column_to_panel(PANEL_script1a, false);
  BUTTON_script_stop = glui_bounds->add_button_to_panel(PANEL_script1a, _("Stop"), SCRIPT_STOP, Script_CB);
  BUTTON_script_stop->disable();

  CHECKBOX_defer = glui_bounds->add_checkbox_to_panel(PANEL_record, _("Turn off file loading while recording"), &defer_file_loading,
    SCRIPT_FILE_LOADING, Script_CB);

  PANEL_run = glui_bounds->add_panel_to_panel(PANEL_script1, _("Run"), true);
  PANEL_script1b = glui_bounds->add_panel_to_panel(PANEL_run, "", false);
  BUTTON_script_runscript = glui_bounds->add_button_to_panel(PANEL_script1b, _("Run script"), SCRIPT_RUNSCRIPT, Script_CB);
  glui_bounds->add_column_to_panel(PANEL_script1b, false);
  CHECKBOX_script_step = glui_bounds->add_checkbox_to_panel(PANEL_run, _("Step through script"), &script_step, SCRIPT_STEP, Script_CB);
  BUTTON_step = glui_bounds->add_button_to_panel(PANEL_run, _("Next"), SCRIPT_STEP_NOW, Script_CB);
  update_script_step();
  glui_bounds->add_button_to_panel(PANEL_run, _("Cancel script"), SCRIPT_CANCEL_NOW, Script_CB);

  LIST_scriptlist = glui_bounds->add_listbox_to_panel(PANEL_script1b, _("Select:"), &script_index, SCRIPT_LIST, Script_CB);
  {
    scriptfiledata *scriptfile;

    for(scriptfile = first_scriptfile.next; scriptfile->next != NULL; scriptfile = scriptfile->next){
      char *file;
      int len;

      file = scriptfile->file;
      if(file == NULL)continue;
      if(file_exists(file) == 0)continue;
      len = strlen(file);
      if(len <= 0)continue;

      LIST_scriptlist->add_item(scriptfile->id, file);
    }
    Script_CB(SCRIPT_LIST);
  }

  ROLLOUT_config = glui_bounds->add_rollout_to_panel(PANEL_files, "Config", false, CONFIG_ROLLOUT, File_Rollout_CB);
  ADDPROCINFO(fileprocinfo, nfileprocinfo, ROLLOUT_config, CONFIG_ROLLOUT);

  PANEL_script2a = glui_bounds->add_panel_to_panel(ROLLOUT_config, "", false);
  EDIT_ini = glui_bounds->add_edittext_to_panel(PANEL_script2a, "suffix:", GLUI_EDITTEXT_TEXT, script_inifile_suffix, SCRIPT_EDIT_INI, Script_CB);
  glui_bounds->add_column_to_panel(PANEL_script2a, false);
  BUTTON_script_setsuffix = glui_bounds->add_button_to_panel(PANEL_script2a, _("Set"), SCRIPT_SETSUFFIX, Script_CB);
  glui_bounds->add_column_to_panel(PANEL_script2a, false);
  BUTTON_script_saveini = glui_bounds->add_button_to_panel(PANEL_script2a, _("Save:"), SCRIPT_SAVEINI, Script_CB);
  Script_CB(SCRIPT_EDIT_INI);

  PANEL_script2b = glui_bounds->add_panel_to_panel(ROLLOUT_config, "", false);
  ini_index = -2;
  LIST_ini_list = glui_bounds->add_listbox_to_panel(PANEL_script2b, _("Select:"), &ini_index);
  {
    inifiledata *inifile;

    for(inifile = first_inifile.next; inifile->next != NULL; inifile = inifile->next){
      if(inifile->file != NULL&&file_exists(inifile->file) == 1){
        if(ini_index == -2)ini_index = inifile->id;
        LIST_ini_list->add_item(inifile->id, inifile->file);
      }
    }
  }
  glui_bounds->add_column_to_panel(PANEL_script2b, false);
  BUTTON_ini_load = glui_bounds->add_button_to_panel(PANEL_script2b, _("Load"), SCRIPT_LOADINI, Script_CB);

  PANEL_script3 = glui_bounds->add_panel_to_panel(ROLLOUT_script, _("Render"), true);
  EDIT_renderdir = glui_bounds->add_edittext_to_panel(PANEL_script3, _("directory:"),
    GLUI_EDITTEXT_TEXT, script_renderdir, SCRIPT_RENDER_DIR, Script_CB);
  EDIT_renderdir->set_w(260);
  PANEL_script1c = glui_bounds->add_panel_to_panel(PANEL_script3, "", false);
  BUTTON_script_render = glui_bounds->add_button_to_panel(PANEL_script1c, _("Render"), SCRIPT_RENDER, Script_CB);
  glui_bounds->add_column_to_panel(PANEL_script1c, false);
  EDIT_rendersuffix = glui_bounds->add_edittext_to_panel(PANEL_script1c, _("suffix:"),
    GLUI_EDITTEXT_TEXT, script_renderfilesuffix, SCRIPT_RENDER_SUFFIX, Script_CB);
  EDIT_rendersuffix->set_w(130);
  Script_CB(SCRIPT_RENDER_SUFFIX);


  PANEL_bounds = glui_bounds->add_panel("Bounds",true);
  ROLLOUT_filebounds = glui_bounds->add_rollout_to_panel(PANEL_bounds,"Data", false, FILEBOUNDS_ROLLOUT, File_Rollout_CB);
  ADDPROCINFO(fileprocinfo, nfileprocinfo, ROLLOUT_filebounds, FILEBOUNDS_ROLLOUT);
  
  /*  zone (cfast) */

  if(nzoneinfo>0){
    ROLLOUT_zone_bound = glui_bounds->add_rollout_to_panel(ROLLOUT_filebounds,"Upper layer temperature",false,ZONE_ROLLOUT,Bound_Rollout_CB);
    ADDPROCINFO(boundprocinfo, nboundprocinfo, ROLLOUT_zone_bound, ZONE_ROLLOUT);

    PANEL_zone_a = glui_bounds->add_panel_to_panel(ROLLOUT_zone_bound,"",GLUI_PANEL_NONE);

    EDIT_zone_min = glui_bounds->add_edittext_to_panel(PANEL_zone_a,"",GLUI_EDITTEXT_FLOAT,&zonemin,ZONEVALMIN,Slice_CB);
    if(setzonemin==0){
      EDIT_zone_min->disable();
    }
    glui_bounds->add_column_to_panel(PANEL_zone_a,false);

    RADIO_zone_setmin = glui_bounds->add_radiogroup_to_panel(PANEL_zone_a,&setzonemin,SETZONEVALMIN,Slice_CB);
    RADIOBUTTON_zone_permin=glui_bounds->add_radiobutton_to_group(RADIO_zone_setmin,_("percentile min"));
    glui_bounds->add_radiobutton_to_group(RADIO_zone_setmin,_("set min"));
    glui_bounds->add_radiobutton_to_group(RADIO_zone_setmin,_("global min"));

    PANEL_zone_b = glui_bounds->add_panel_to_panel(ROLLOUT_zone_bound,"",GLUI_PANEL_NONE);

    EDIT_zone_max = glui_bounds->add_edittext_to_panel(PANEL_zone_b,"",GLUI_EDITTEXT_FLOAT,&zonemax,ZONEVALMAX,Slice_CB);
    if(setzonemax==0){
      EDIT_zone_max->disable();
    }
    glui_bounds->add_column_to_panel(PANEL_zone_b,false);

    RADIO_zone_setmax = glui_bounds->add_radiogroup_to_panel(PANEL_zone_b,&setzonemax,SETZONEVALMAX,Slice_CB);
    RADIOBUTTON_zone_permax=glui_bounds->add_radiobutton_to_group(RADIO_zone_setmax,_("percentile max"));
    glui_bounds->add_radiobutton_to_group(RADIO_zone_setmax,_("set max"));
    glui_bounds->add_radiobutton_to_group(RADIO_zone_setmax,_("global max"));
    
    RADIOBUTTON_zone_permin->disable();
    RADIOBUTTON_zone_permax->disable();
    Slice_CB(SETZONEVALMIN);
    Slice_CB(SETZONEVALMAX);
  }

  /*  3d smoke   */

  if(nsmoke3dinfo>0||nvolrenderinfo>0){
    ROLLOUT_smoke3d = glui_bounds->add_rollout_to_panel(ROLLOUT_filebounds,"3D smoke",false,SMOKE3D_ROLLOUT,Bound_Rollout_CB);
    ADDPROCINFO(boundprocinfo, nboundprocinfo, ROLLOUT_smoke3d, SMOKE3D_ROLLOUT);
  }

  /*  Boundary File Bounds   */

  if(npatchinfo>0){
    glui_active=1;
    ROLLOUT_bound = glui_bounds->add_rollout_to_panel(ROLLOUT_filebounds,"Boundary",false,BOUNDARY_ROLLOUT,Bound_Rollout_CB);
    ADDPROCINFO(boundprocinfo, nboundprocinfo, ROLLOUT_bound, BOUNDARY_ROLLOUT);

    RADIO_bf = glui_bounds->add_radiogroup_to_panel(ROLLOUT_bound,&list_patch_index,FILETYPEINDEX,Bound_CB);
    nradio=0;
    for(i=0;i<npatchinfo;i++){
      patchdata *patchi;

      patchi = patchinfo + i;
      if(patchi->firstshort==1)nradio++;
    }
    CHECKBOX_cache_boundarydata=glui_bounds->add_checkbox_to_panel(ROLLOUT_bound,_("Cache boundary data"),&cache_boundarydata,CACHE_BOUNDARYDATA,Bound_CB);
    CHECKBOX_showpatch_both=glui_bounds->add_checkbox_to_panel(ROLLOUT_bound,_("Display exterior data"),&showpatch_both,SHOWPATCH_BOTH,Bound_CB);
    ROLLOUT_outputpatchdata = glui_bounds->add_rollout_to_panel(ROLLOUT_bound,"Ouput data",false);
    glui_bounds->add_checkbox_to_panel(ROLLOUT_outputpatchdata,_("Output data to file"),&output_patchdata);

    PANEL_outputpatchdata = glui_bounds->add_panel_to_panel(ROLLOUT_outputpatchdata,"",GLUI_PANEL_NONE);

    glui_bounds->add_spinner_to_panel(PANEL_outputpatchdata,"tmin",GLUI_SPINNER_FLOAT,&patchout_tmin);
    glui_bounds->add_spinner_to_panel(PANEL_outputpatchdata,"xmin",GLUI_SPINNER_FLOAT,&patchout_xmin);
    glui_bounds->add_spinner_to_panel(PANEL_outputpatchdata,"ymin",GLUI_SPINNER_FLOAT,&patchout_ymin);
    glui_bounds->add_spinner_to_panel(PANEL_outputpatchdata,"zmin",GLUI_SPINNER_FLOAT,&patchout_zmin);

    glui_bounds->add_column_to_panel(PANEL_outputpatchdata,false);

    glui_bounds->add_spinner_to_panel(PANEL_outputpatchdata,"tmax",GLUI_SPINNER_FLOAT,&patchout_tmax);
    glui_bounds->add_spinner_to_panel(PANEL_outputpatchdata,"xmax",GLUI_SPINNER_FLOAT,&patchout_xmax);
    glui_bounds->add_spinner_to_panel(PANEL_outputpatchdata,"ymax",GLUI_SPINNER_FLOAT,&patchout_ymax);
    glui_bounds->add_spinner_to_panel(PANEL_outputpatchdata,"zmax",GLUI_SPINNER_FLOAT,&patchout_zmax);
    

    if(nradio>1){
      for(i=0;i<npatchinfo;i++){
        patchdata *patchi;

        patchi = patchinfo + i;
        if(patchi->firstshort==1)glui_bounds->add_radiobutton_to_group(RADIO_bf,patchi->label.shortlabel);
      }
      if(activate_threshold==1){
        glui_bounds->add_separator_to_panel(ROLLOUT_bound);
        CHECKBOX_showchar=glui_bounds->add_checkbox_to_panel(ROLLOUT_bound,_("Show temp threshold"),&vis_threshold,SHOWCHAR,Bound_CB);
        CHECKBOX_showonlychar=glui_bounds->add_checkbox_to_panel(ROLLOUT_bound,_("Show only temp threshold"),&vis_onlythreshold,SHOWCHAR,Bound_CB);
        {
          char label[256];

          strcpy(label,"Temperature threshold (");
          strcat(label,degC);
          strcat(label,") ");
          glui_bounds->add_spinner_to_panel(ROLLOUT_bound,label,GLUI_SPINNER_FLOAT,&temp_threshold);
        }
        Bound_CB(SHOWCHAR);
      }
      glui_bounds->add_column_to_panel(ROLLOUT_bound,false);
    }
    else{
      if(activate_threshold==1){
        CHECKBOX_showchar=glui_bounds->add_checkbox_to_panel(ROLLOUT_bound,_("Show temp threshold"),&vis_threshold,SHOWCHAR,Bound_CB);
        CHECKBOX_showonlychar=glui_bounds->add_checkbox_to_panel(ROLLOUT_bound,_("Show only threshold"),&vis_onlythreshold,SHOWCHAR,Bound_CB);
        {
          char label[256];

          strcpy(label,"Temperature (");
          strcat(label,degC);
          strcat(label,") ");
          glui_bounds->add_spinner_to_panel(ROLLOUT_bound,label,GLUI_SPINNER_FLOAT,&temp_threshold);
        }
        Bound_CB(SHOWCHAR);
      }
    }

    boundmenu(&ROLLOUT_boundary,NULL,ROLLOUT_bound,"Reload Boundary File(s)",
      &EDIT_patch_min,&EDIT_patch_max,&RADIO_patch_setmin,&RADIO_patch_setmax,
      &CHECKBOX_patch_setchopmin, &CHECKBOX_patch_setchopmax,
      &EDIT_patch_chopmin, &EDIT_patch_chopmax,
      &STATIC_bound_min_unit,&STATIC_bound_max_unit,
      &STATIC_bound_cmin_unit,&STATIC_bound_cmax_unit,
      &BUTTON_updatebound, &BUTTON_reloadbound,

      &setpatchmin,&setpatchmax,&patchmin,&patchmax,
      &setpatchchopmin, &setpatchchopmax,
      &patchchopmin, &patchchopmax,
      UPDATERELOAD_BOUNDS,DONT_TRUNCATE_BOUNDS,
      Bound_CB);
    updatepatchlistindex2(patchinfo->label.shortlabel);
    update_hidepatchsurface();
    Bound_CB(CACHE_BOUNDARYDATA);
  }
  
  /*  Iso File Load Bounds   */

  if(nisoinfo>0){
    ROLLOUT_iso = glui_bounds->add_rollout_to_panel(ROLLOUT_filebounds,"Isosurface",false,ISO_ROLLOUT,Bound_Rollout_CB);
    ADDPROCINFO(boundprocinfo, nboundprocinfo, ROLLOUT_iso, ISO_ROLLOUT);
    
    SPINNER_isopointsize=glui_bounds->add_spinner_to_panel(ROLLOUT_iso,_("Point size"),GLUI_SPINNER_FLOAT,&isopointsize);
    SPINNER_isopointsize->set_float_limits(1.0,10.0);

    SPINNER_isolinewidth=glui_bounds->add_spinner_to_panel(ROLLOUT_iso,_("Line width"),GLUI_SPINNER_FLOAT,&isolinewidth);
    SPINNER_isolinewidth->set_float_limits(1.0,10.0);

    visAIso=showtrisurface*1+showtrioutline*2+showtripoints*4;
    CHECKBOX_showtrisurface=glui_bounds->add_checkbox_to_panel(ROLLOUT_iso,_("Solid"),&showtrisurface,ISO_SURFACE,Iso_CB);
    CHECKBOX_showtrioutline=glui_bounds->add_checkbox_to_panel(ROLLOUT_iso,_("Outline"),&showtrioutline,ISO_OUTLINE,Iso_CB);
    CHECKBOX_showtripoints=glui_bounds->add_checkbox_to_panel(ROLLOUT_iso,_("Points"),&showtripoints,ISO_POINTS,Iso_CB);

    CHECKBOX_transparentflag2=glui_bounds->add_checkbox_to_panel(ROLLOUT_iso,_("Use transparency:"),&use_transparency_data,DATA_transparent,Slice_CB);
#ifdef pp_BETA 
    CHECKBOX_sort2=glui_bounds->add_checkbox_to_panel(ROLLOUT_iso,_("Sort transparent surfaces:"),&sort_iso_triangles,SORT_SURFACES,Slice_CB);
    CHECKBOX_smooth2=glui_bounds->add_checkbox_to_panel(ROLLOUT_iso,_("Smooth surfaces:"),&smoothtrinormal,SMOOTH_SURFACES,Slice_CB);
#endif
    SPINNER_labels_transparency_data2=glui_bounds->add_spinner_to_panel(ROLLOUT_iso,
        _("transparency level"),GLUI_SPINNER_FLOAT,&transparent_level,TRANSPARENTLEVEL,Slice_CB);
    SPINNER_labels_transparency_data2->set_w(0);
    SPINNER_labels_transparency_data2->set_float_limits(0.0,1.0,GLUI_LIMIT_CLAMP);
  }

  /* Particle File Bounds  */

//SVEXTERN part5prop *part5propinfo;
//SVEXTERN int npart5prop;

  if(npartinfo>0&&nevac!=npartinfo){
    glui_active=1;
    ROLLOUT_part = glui_bounds->add_rollout_to_panel(ROLLOUT_filebounds,"Particle",false,PART_ROLLOUT,Bound_Rollout_CB);
    ADDPROCINFO(boundprocinfo, nboundprocinfo, ROLLOUT_part, PART_ROLLOUT);
    
    if(npart5prop>0){
      ipart5prop=0;
      ipart5prop_old=0;
      RADIO_part5 = glui_bounds->add_radiogroup_to_panel(ROLLOUT_part,&ipart5prop,FILETYPEINDEX,PART_CB);

      for(i=0;i<npart5prop;i++){
        part5prop *partpropi;

        partpropi = part5propinfo + i;
        glui_bounds->add_radiobutton_to_group(RADIO_part5,partpropi->label->shortlabel);
      }

      glui_bounds->add_column_to_panel(ROLLOUT_part,false);

      {
        part5prop *propi;

        propi = part5propinfo;
        setpartmin=propi->setvalmin;
        setpartmax=propi->setvalmax;
        setpartchopmin=propi->setchopmin;
        setpartchopmax=propi->setchopmax;
      }

      PART_CB(FILETYPEINDEX);
      PART_CB(SETVALMIN);
      PART_CB(SETVALMAX);
    }

    {
      char boundmenulabel[100];

      strcpy(boundmenulabel,"Reload Particle File(s)");
      boundmenu(&ROLLOUT_part_bound,&ROLLOUT_part_chop,ROLLOUT_part,boundmenulabel,
        &EDIT_part_min,&EDIT_part_max,&RADIO_part_setmin,&RADIO_part_setmax,
        &CHECKBOX_part_setchopmin, &CHECKBOX_part_setchopmax,
        &EDIT_part_chopmin, &EDIT_part_chopmax,
        &STATIC_part_min_unit,&STATIC_part_max_unit,
        NULL,NULL,
        NULL,NULL,
        &setpartmin,&setpartmax,&partmin,&partmax,
        &setpartchopmin,&setpartchopmax,&partchopmin,&partchopmax,
        RELOAD_BOUNDS,DONT_TRUNCATE_BOUNDS,
        PART_CB);
        PART_CB(FILETYPEINDEX);
        if(partinfo!=NULL&&partinfo->version==0){
          SPINNER_partpointstep=glui_bounds->add_spinner_to_panel(ROLLOUT_part,_("Point skip"),GLUI_SPINNER_INT,
            &partpointskip,FRAMELOADING,PART_CB);
          SPINNER_partpointstep->set_int_limits(0,100);
        }
        SPINNER_partpointsize=glui_bounds->add_spinner_to_panel(ROLLOUT_part,_("Particle size"),GLUI_SPINNER_FLOAT,
          &partpointsize);
        SPINNER_partpointsize->set_float_limits(1.0,100.0);
        SPINNER_streaklinewidth=glui_bounds->add_spinner_to_panel(ROLLOUT_part,_("Streak line width"),GLUI_SPINNER_FLOAT,
          &streaklinewidth);
        SPINNER_streaklinewidth->set_float_limits(1.0,100.0);

        SPINNER_partstreaklength=glui_bounds->add_spinner_to_panel(ROLLOUT_part,_("Streak length (s)"),GLUI_SPINNER_FLOAT,
          &float_streak5value,STREAKLENGTH,PART_CB);
        SPINNER_partstreaklength->set_float_limits(0.0,tmax_part);
        
        CHECKBOX_showtracer=glui_bounds->add_checkbox_to_panel(ROLLOUT_part,_("Always show tracers"),&show_tracers_always,
          TRACERS,PART_CB);
    }
  }

  if(nevac>0){
    glui_active=1;
    ROLLOUT_evac = glui_bounds->add_rollout_to_panel(ROLLOUT_filebounds,"Evacuation",false,EVAC_ROLLOUT,Bound_Rollout_CB);
    ADDPROCINFO(boundprocinfo, nboundprocinfo, ROLLOUT_evac, EVAC_ROLLOUT);
    
    glui_bounds->add_checkbox_to_panel(ROLLOUT_evac,_("Select avatar"),&select_avatar);
    CHECKBOX_show_evac_slices=glui_bounds->add_checkbox_to_panel(ROLLOUT_evac,"Show slice menus",&show_evac_slices,SHOW_EVAC_SLICES,Slice_CB);
    PANEL_evac_direction=glui_bounds->add_panel_to_panel(ROLLOUT_evac,_("Direction vectors"));
    CHECKBOX_constant_coloring=glui_bounds->add_checkbox_to_panel(PANEL_evac_direction,_("Constant coloring"),&constant_evac_coloring,SHOW_EVAC_SLICES,Slice_CB);
    CHECKBOX_data_coloring=glui_bounds->add_checkbox_to_panel(PANEL_evac_direction,_("Data coloring"),&data_evac_coloring,DATA_EVAC_COLORING,Slice_CB);
    CHECKBOX_show_evac_color=glui_bounds->add_checkbox_to_panel(PANEL_evac_direction,_("Show colorbar (when data coloring)"),&show_evac_colorbar,SHOW_EVAC_SLICES,Slice_CB);
    glui_bounds->add_checkbox_to_panel(ROLLOUT_evac,_("View from selected Avatar"),&view_from_selected_avatar);
  }

  /* Plot3D file bounds */

  if(nplot3dinfo>0){
    glui_active=1;
    ROLLOUT_plot3d = glui_bounds->add_rollout_to_panel(ROLLOUT_filebounds,"Plot3D",false,PLOT3D_ROLLOUT,Bound_Rollout_CB);
    ADDPROCINFO(boundprocinfo, nboundprocinfo, ROLLOUT_plot3d, PLOT3D_ROLLOUT);

    RADIO_p3 = glui_bounds->add_radiogroup_to_panel(ROLLOUT_plot3d,&list_p3_index,FILETYPEINDEX,PLOT3D_CB);
    for(i=0;i<mxplot3dvars;i++){
      glui_bounds->add_radiobutton_to_group(RADIO_p3,plot3dinfo[0].label[i].shortlabel);
    }
    CHECKBOX_cache_qdata=glui_bounds->add_checkbox_to_panel(ROLLOUT_plot3d,_("Cache PLOT3D PLOT3D data"),&cache_qdata,UNLOAD_QDATA,PLOT3D_CB);

    PANEL_pan3 = glui_bounds->add_panel_to_panel(ROLLOUT_plot3d,"",GLUI_PANEL_NONE);
    PANEL_vector = glui_bounds->add_panel_to_panel(PANEL_pan3,_("Vector"));
    glui_bounds->add_checkbox_to_panel(PANEL_vector,_("Show vectors"),&visVector,UPDATEPLOT,PLOT3D_CB);
    SPINNER_plot3d_vectorpointsize=glui_bounds->add_spinner_to_panel(PANEL_vector,_("Point size"),GLUI_SPINNER_FLOAT,&vectorpointsize,UPDATE_VECTOR,PLOT3D_CB);
    SPINNER_plot3d_vectorpointsize->set_float_limits(1.0,10.0);
    SPINNER_plot3d_vectorlinewidth=glui_bounds->add_spinner_to_panel(PANEL_vector,_("Vector width"),GLUI_SPINNER_FLOAT,&vectorlinewidth,UPDATE_VECTOR,PLOT3D_CB);
    SPINNER_plot3d_vectorlinewidth->set_float_limits(1.0,10.0);
    SPINNER_plot3d_vectorlinelength=glui_bounds->add_spinner_to_panel(PANEL_vector,_("Vector length"),GLUI_SPINNER_FLOAT,&vecfactor,UPDATE_VECTOR,PLOT3D_CB);
    SPINNER_plot3d_vectorlinelength->set_float_limits(0.0,20.0);
    SPINNER_plot3dvectorskip=glui_bounds->add_spinner_to_panel(PANEL_vector,_("Vector skip"),GLUI_SPINNER_INT,&vectorskip,PLOT3D_VECTORSKIP,PLOT3D_CB);
    SPINNER_plot3dvectorskip->set_int_limits(1,4);

    PANEL_isosurface = glui_bounds->add_panel_to_panel(ROLLOUT_plot3d,"Isosurface");
    PANEL_pan1 = glui_bounds->add_panel_to_panel(PANEL_isosurface,"",GLUI_PANEL_NONE);

    glui_bounds->add_checkbox_to_panel(PANEL_pan1,"Show isosurface",&visiso,PLOTISO,PLOT3D_CB);
    SPINNER_plot3dpointsize=glui_bounds->add_spinner_to_panel(PANEL_pan1,_("Point size"),GLUI_SPINNER_FLOAT,
      &plot3dpointsize);
    SPINNER_plot3dpointsize->set_float_limits(1.0,10.0);

    SPINNER_plot3dlinewidth=glui_bounds->add_spinner_to_panel(PANEL_pan1,_("Line width"),GLUI_SPINNER_FLOAT,
      &plot3dlinewidth);
    SPINNER_plot3dlinewidth->set_float_limits(1.0,10.0);
//    glui_bounds->add_column_to_panel(PANEL_isosurface);
    PANEL_pan2 = glui_bounds->add_panel_to_panel(PANEL_isosurface,"",GLUI_PANEL_NONE);
    RADIO_plot3d_isotype=glui_bounds->add_radiogroup_to_panel(PANEL_pan2,&p3dsurfacetype,PLOTISOTYPE,PLOT3D_CB);
    RADIOBUTTON_plot3d_iso_hidden=glui_bounds->add_radiobutton_to_group(RADIO_plot3d_isotype,_("Hidden"));
    glui_bounds->add_radiobutton_to_group(RADIO_plot3d_isotype,_("Solid"));
    glui_bounds->add_radiobutton_to_group(RADIO_plot3d_isotype,_("Outline"));
    glui_bounds->add_radiobutton_to_group(RADIO_plot3d_isotype,_("Points"));
    RADIOBUTTON_plot3d_iso_hidden->disable();


    p3min_temp=p3min[0];
    p3max_temp=p3max[0];
    p3chopmin_temp=p3chopmin[0];
    p3chopmax_temp=p3chopmax[0];
    glui_bounds->add_column_to_panel(ROLLOUT_plot3d,false);
    boundmenu(NULL,NULL,ROLLOUT_plot3d,"Reload Plot3D file(s)",
      &EDIT_p3_min,&EDIT_p3_max,&RADIO_p3_setmin,&RADIO_p3_setmax,
      &CHECKBOX_p3_setchopmin, &CHECKBOX_p3_setchopmax,
      &EDIT_p3_chopmin, &EDIT_p3_chopmax,
      &STATIC_plot3d_min_unit,&STATIC_plot3d_max_unit,
      &STATIC_plot3d_cmin_unit,&STATIC_plot3d_cmax_unit,
      NULL,NULL,
      &setp3min_temp,&setp3max_temp,&p3min_temp,&p3max_temp,
      &setp3chopmin_temp, &setp3chopmax_temp,&p3chopmin_temp,&p3chopmax_temp,
      RELOAD_BOUNDS,TRUNCATE_BOUNDS,
      PLOT3D_CB);
    PLOT3D_CB(FILETYPEINDEX);
    PLOT3D_CB(UNLOAD_QDATA);
  }

  /*  Slice File Bounds   */

  if(nsliceinfo>0){
    int index;

    glui_active=1;
    ROLLOUT_slice = glui_bounds->add_rollout_to_panel(ROLLOUT_filebounds,"Slice",false,SLICE_ROLLOUT,Bound_Rollout_CB);
    ADDPROCINFO(boundprocinfo, nboundprocinfo, ROLLOUT_slice, SLICE_ROLLOUT);

    RADIO_slice = glui_bounds->add_radiogroup_to_panel(ROLLOUT_slice,&list_slice_index,FILETYPEINDEX,Slice_CB);

    index=0;
    for(i=0;i<nsliceinfo;i++){
      if(sliceinfo[i].firstshort==1){
        GLUI_RadioButton *RADIOBUTTON_slicetype;

        RADIOBUTTON_slicetype=glui_bounds->add_radiobutton_to_group(RADIO_slice,sliceinfo[i].label.shortlabel);
        if(strcmp(sliceinfo[i].label.shortlabel,_("Fire line"))==0){
          RADIOBUTTON_slicetype->disable();
          fire_line_index=index;
        }
        index++;
      }
    }

    glui_bounds->add_column_to_panel(ROLLOUT_slice,false);

    boundmenu(&ROLLOUT_slice_bound,&ROLLOUT_slice_chop,ROLLOUT_slice,"Reload Slice File(s)",
      &EDIT_slice_min,&EDIT_slice_max,&RADIO_slice_setmin,&RADIO_slice_setmax,
      &CHECKBOX_slice_setchopmin, &CHECKBOX_slice_setchopmax,
      &EDIT_slice_chopmin, &EDIT_slice_chopmax,
      &STATIC_slice_min_unit,&STATIC_slice_max_unit,
      &STATIC_slice_cmin_unit,&STATIC_slice_cmax_unit,
      NULL,NULL,
      &setslicemin,&setslicemax,&slicemin,&slicemax,
      &setslicechopmin, &setslicechopmax,
      &slicechopmin, &slicechopmax,
      UPDATE_BOUNDS,DONT_TRUNCATE_BOUNDS,
      Slice_CB);
    SPINNER_transparent_level=glui_bounds->add_spinner_to_panel(ROLLOUT_slice,_("Transparent level"),GLUI_SPINNER_FLOAT,&transparent_level,TRANSPARENTLEVEL,Slice_CB);
    SPINNER_transparent_level->set_float_limits(0.0,1.0);

    PANEL_slice_average=glui_bounds->add_panel_to_panel(ROLLOUT_slice,_("Average"));
    CHECKBOX_average_slice=glui_bounds->add_checkbox_to_panel(PANEL_slice_average,_("Average slice data"),&slice_average_flag);
    SPINNER_sliceaverage=glui_bounds->add_spinner_to_panel(PANEL_slice_average,_("Time interval"),GLUI_SPINNER_FLOAT,&slice_average_interval);
    {
      float tttmax=120.0;

      if(view_tstop>tttmax)tttmax=view_tstop;
      SPINNER_sliceaverage->set_float_limits(0.0,tttmax);
    }
    glui_bounds->add_button_to_panel(PANEL_slice_average,"Reload",FILERELOAD,Slice_CB);

    PANEL_slice_vector=glui_bounds->add_panel_to_panel(ROLLOUT_slice,_("Vector"));
    SPINNER_vectorpointsize=glui_bounds->add_spinner_to_panel(PANEL_slice_vector,_("Point size"),GLUI_SPINNER_FLOAT,
      &vectorpointsize,UPDATE_VECTOR,Slice_CB);
    SPINNER_vectorpointsize->set_float_limits(1.0,10.0);
    SPINNER_vectorlinewidth=glui_bounds->add_spinner_to_panel(PANEL_slice_vector,_("Vector width"),GLUI_SPINNER_FLOAT,&vectorlinewidth,UPDATE_VECTOR,Slice_CB);
    SPINNER_vectorlinewidth->set_float_limits(1.0,10.0);
    SPINNER_vectorlinelength=glui_bounds->add_spinner_to_panel(PANEL_slice_vector,_("Vector length"),GLUI_SPINNER_FLOAT,&vecfactor,UPDATE_VECTOR,Slice_CB);
    SPINNER_vectorlinelength->set_float_limits(0.0,20.0);
    SPINNER_slicevectorskip=glui_bounds->add_spinner_to_panel(PANEL_slice_vector,_("Vector skip"),GLUI_SPINNER_INT,&vectorskip,SLICE_VECTORSKIP,Slice_CB);
    SPINNER_slicevectorskip->set_int_limits(1,4);
    CHECKBOX_show_slices_and_vectors=glui_bounds->add_checkbox_to_panel(PANEL_slice_vector,_("Show contours"),&show_slices_and_vectors);

    PANEL_line_contour = glui_bounds->add_panel_to_panel(ROLLOUT_slice,_("Line Contours"));
    slice_line_contour_min=0.0;
    slice_line_contour_max=1.0;
    SPINNER_line_contour_min=glui_bounds->add_spinner_to_panel(PANEL_line_contour,_("Min"),GLUI_SPINNER_FLOAT,
      &slice_line_contour_min,LINE_CONTOUR_VALUE,Slice_CB);
    SPINNER_line_contour_max=glui_bounds->add_spinner_to_panel(PANEL_line_contour,_("Max"),GLUI_SPINNER_FLOAT,
      &slice_line_contour_max,LINE_CONTOUR_VALUE,Slice_CB);
    slice_line_contour_num=1;
    SPINNER_line_contour_num=glui_bounds->add_spinner_to_panel(PANEL_line_contour,_("Number of contours"),GLUI_SPINNER_INT,
      &slice_line_contour_num,LINE_CONTOUR_VALUE,Slice_CB);
    SPINNER_line_contour_width=glui_bounds->add_spinner_to_panel(PANEL_line_contour,_("contour width"),GLUI_SPINNER_FLOAT,&slice_line_contour_width);
    SPINNER_line_contour_width->set_float_limits(1.0,10.0);
      RADIO_contour_type = glui_bounds->add_radiogroup_to_panel(PANEL_line_contour,&slice_contour_type);
    glui_bounds->add_radiobutton_to_group(RADIO_contour_type,"line");
#ifdef _DEBUG
    glui_bounds->add_radiobutton_to_group(RADIO_contour_type,"stepped");
#endif

    BUTTON_update_line_contour=glui_bounds->add_button_to_panel(PANEL_line_contour,_("Update contours"),UPDATE_LINE_CONTOUR_VALUE,Slice_CB);
    glui_bounds->add_checkbox_to_panel(PANEL_line_contour,_("Show contours"),&vis_slice_contours);

    if(n_embedded_meshes>0){
      CHECKBOX_skip_subslice=glui_bounds->add_checkbox_to_panel(ROLLOUT_slice,_("Skip coarse sub-slice"),&skip_slice_in_embedded_mesh);
    }
    if(nfedinfo>0){
      glui_bounds->add_checkbox_to_panel(ROLLOUT_slice,"Regenerate FED data",&regenerate_fed);
    }
    CHECKBOX_research_mode=glui_bounds->add_checkbox_to_panel(ROLLOUT_slice,_("Research display mode"),&research_mode,RESEARCH_MODE,Slice_CB);
    glui_bounds->add_checkbox_to_panel(ROLLOUT_slice,_("Output data to file"),&output_slicedata);
    Slice_CB(FILETYPEINDEX);
  }

  ROLLOUT_time = glui_bounds->add_rollout_to_panel(PANEL_bounds,"Time", false, TIME_ROLLOUT, File_Rollout_CB);
  ADDPROCINFO(fileprocinfo, nfileprocinfo, ROLLOUT_time, TIME_ROLLOUT);

  PANEL_time1a = glui_bounds->add_panel_to_panel(ROLLOUT_time,"",false);
  SPINNER_timebounds=glui_bounds->add_spinner_to_panel(PANEL_time1a,_("Time:"),GLUI_SPINNER_FLOAT,&glui_time);
  glui_bounds->add_column_to_panel(PANEL_time1a,false);
  SPINNER_timebounds->set_float_limits(0.0,3600.0*24);
  BUTTON_SETTIME=glui_bounds->add_button_to_panel(PANEL_time1a,_("Set"),SET_TIME,Time_CB);
  
  PANEL_time2 = glui_bounds->add_panel_to_panel(ROLLOUT_time,_("Data loading"),true);

  PANEL_time2a = glui_bounds->add_panel_to_panel(PANEL_time2,"",false);
  SPINNER_tload_begin=glui_bounds->add_spinner_to_panel(PANEL_time2a,"min time",GLUI_SPINNER_FLOAT,&tload_begin,TBOUNDS,Time_CB);
  glui_bounds->add_column_to_panel(PANEL_time2a,false);
  CHECKBOX_use_tload_begin=glui_bounds->add_checkbox_to_panel(PANEL_time2a,"",&use_tload_begin,TBOUNDS_USE,Time_CB);

  PANEL_time2b = glui_bounds->add_panel_to_panel(PANEL_time2,"",false);
  SPINNER_tload_end=glui_bounds->add_spinner_to_panel(PANEL_time2b,"max time",GLUI_SPINNER_FLOAT,&tload_end,TBOUNDS,Time_CB);
  glui_bounds->add_column_to_panel(PANEL_time2b,false);
  CHECKBOX_use_tload_end=glui_bounds->add_checkbox_to_panel(PANEL_time2b,"",&use_tload_end,TBOUNDS_USE,Time_CB);

  PANEL_time2c = glui_bounds->add_panel_to_panel(PANEL_time2,"",false);
  SPINNER_tload_skip=glui_bounds->add_spinner_to_panel(PANEL_time2c,_("frame skip"),GLUI_SPINNER_INT,&tload_skip,TBOUNDS,Time_CB);
  glui_bounds->add_column_to_panel(PANEL_time2c,false);
  CHECKBOX_use_tload_skip=glui_bounds->add_checkbox_to_panel(PANEL_time2c,"",&use_tload_skip,TBOUNDS_USE,Time_CB);
  SPINNER_tload_skip->set_int_limits(0,100);
  
  BUTTON_RELOAD=glui_bounds->add_button_to_panel(PANEL_time2,_("Reload"),RELOAD_DATA,Time_CB);

  Time_CB(TBOUNDS_USE);
  
#ifdef pp_MEMDEBUG
  PANEL_memcheck = glui_bounds->add_rollout(_("Memory Check"),false);
  list_memcheck_index=0;
  RADIO_memcheck = glui_bounds->add_radiogroup_to_panel(PANEL_memcheck,&list_memcheck_index,MEMCHECK,Memcheck_CB);
  glui_bounds->add_radiobutton_to_group(RADIO_memcheck,"Unlimited");
  glui_bounds->add_radiobutton_to_group(RADIO_memcheck,"1 GB");
  glui_bounds->add_radiobutton_to_group(RADIO_memcheck,"2 GB");
  #ifdef BIT64
  glui_bounds->add_radiobutton_to_group(RADIO_memcheck,"4 GB");
  glui_bounds->add_radiobutton_to_group(RADIO_memcheck,"8 GB");
  #endif
#endif

  glui_bounds->add_button(_("Save settings"), SAVE_SETTINGS, Bounds_DLG_CB);
  glui_bounds->add_button(_("Close"), CLOSE, Bounds_DLG_CB);

  glui_bounds->set_main_gfx_window( main_window );
}

/* ------------------ compress_onoff ------------------------ */

extern "C" void compress_onoff(int flag){
  switch(flag){
    case OFF:
      if(BUTTON_compress!=NULL)BUTTON_compress->disable();
      if(CHECKBOX_overwrite_all!=NULL)CHECKBOX_overwrite_all->disable();
      if(CHECKBOX_erase_all!=NULL)CHECKBOX_erase_all->disable();
      if(CHECKBOX_multi_task!=NULL)CHECKBOX_multi_task->disable();
    case ON:
      if(BUTTON_compress!=NULL)BUTTON_compress->enable();
      if(CHECKBOX_overwrite_all!=NULL)CHECKBOX_overwrite_all->enable();
      if(CHECKBOX_erase_all!=NULL)CHECKBOX_erase_all->enable();
      if(CHECKBOX_multi_task!=NULL)CHECKBOX_multi_task->enable();
    default:
      ASSERT(FFALSE);
      break;
  }
}

/* ------------------ boundmenu ------------------------ */

void boundmenu(GLUI_Rollout **bound_rollout,GLUI_Rollout **chop_rollout, GLUI_Panel *PANEL_panel, char *button_title,
          GLUI_EditText **EDIT_con_min,GLUI_EditText **EDIT_con_max,
          GLUI_RadioGroup **RADIO_con_setmin,GLUI_RadioGroup **RADIO_con_setmax,
          GLUI_Checkbox **CHECKBOX_con_setchopmin, GLUI_Checkbox **CHECKBOX_con_setchopmax,
          GLUI_EditText **EDIT_con_chopmin, GLUI_EditText **EDIT_con_chopmax,
          GLUI_StaticText **STATIC_con_min_unit,GLUI_StaticText **STATIC_con_max_unit,
          GLUI_StaticText **STATIC_con_cmin_unit,GLUI_StaticText **STATIC_con_cmax_unit,
          GLUI_Button **BUTTON_update, GLUI_Button **BUTTON_reload,

          int *setminval, int *setmaxval,
          float *minval, float *maxval,
          int *setchopminval, int *setchopmaxval,
          float *chopminval, float *chopmaxval,
          int updatebounds,
          int truncatebounds,
          GLUI_Update_CB FILE_CB){

  GLUI_Panel *PANEL_a,*PANEL_b,*PANEL_c;
  GLUI_Rollout *PANEL_e=NULL,*PANEL_g=NULL;
  GLUI_Panel *PANEL_f=NULL, *PANEL_h=NULL;

  PANEL_g = glui_bounds->add_rollout_to_panel(PANEL_panel,_("Bound data"),false);
  if(bound_rollout!=NULL)*bound_rollout=PANEL_g;

  PANEL_a = glui_bounds->add_panel_to_panel(PANEL_g,"",GLUI_PANEL_NONE);

  *EDIT_con_min = glui_bounds->add_edittext_to_panel(PANEL_a,"",GLUI_EDITTEXT_FLOAT,minval,VALMIN,FILE_CB);
  if(*setminval==0){
    (*EDIT_con_min)->disable();
  }
  glui_bounds->add_column_to_panel(PANEL_a,false);
  
  if(STATIC_con_min_unit!=NULL){
    *STATIC_con_min_unit=glui_bounds->add_statictext_to_panel(PANEL_a,"xx");
    glui_bounds->add_column_to_panel(PANEL_a,false);
    (*STATIC_con_min_unit)->set_w(10);
  }

  *RADIO_con_setmin = glui_bounds->add_radiogroup_to_panel(PANEL_a,setminval,SETVALMIN,FILE_CB);
  glui_bounds->add_radiobutton_to_group(*RADIO_con_setmin,_("percentile min"));
  glui_bounds->add_radiobutton_to_group(*RADIO_con_setmin,_("set min"));
  glui_bounds->add_radiobutton_to_group(*RADIO_con_setmin,_("global min"));
  
  PANEL_b = glui_bounds->add_panel_to_panel(PANEL_g,"",GLUI_PANEL_NONE);

  *EDIT_con_max = glui_bounds->add_edittext_to_panel(PANEL_b,"",GLUI_EDITTEXT_FLOAT,maxval,VALMAX,FILE_CB);
  if(*setminval==0){
    (*EDIT_con_max)->disable();
  }
  glui_bounds->add_column_to_panel(PANEL_b,false);

  if(STATIC_con_max_unit!=NULL){
    *STATIC_con_max_unit=glui_bounds->add_statictext_to_panel(PANEL_b,"yy");
    glui_bounds->add_column_to_panel(PANEL_b,false);
    (*STATIC_con_max_unit)->set_w(10);
  }

  *RADIO_con_setmax = glui_bounds->add_radiogroup_to_panel(PANEL_b,setmaxval,SETVALMAX,FILE_CB);
  glui_bounds->add_radiobutton_to_group(*RADIO_con_setmax,_("percentile max"));
  glui_bounds->add_radiobutton_to_group(*RADIO_con_setmax,_("set max"));
  glui_bounds->add_radiobutton_to_group(*RADIO_con_setmax,_("global max"));

  PANEL_c = glui_bounds->add_panel_to_panel(PANEL_g,"",GLUI_PANEL_NONE);
  
  if(updatebounds==UPDATE_BOUNDS){
    glui_bounds->add_button_to_panel(PANEL_c,_("Update"),FILEUPDATE,FILE_CB);
  }
  else if(updatebounds==RELOAD_BOUNDS){
    glui_bounds->add_button_to_panel(PANEL_c,button_title,FILERELOAD,FILE_CB);
  }
  else{
    BUTTON_updatebound=glui_bounds->add_button_to_panel(PANEL_c,_("Update using cached data"),FILEUPDATEDATA,FILE_CB);
    BUTTON_reloadbound=glui_bounds->add_button_to_panel(PANEL_c,button_title,FILERELOAD,FILE_CB);
  }

  if(EDIT_con_chopmin!=NULL&&EDIT_con_chopmax!=NULL&&CHECKBOX_con_setchopmin!=NULL&&CHECKBOX_con_setchopmax!=NULL){
    PANEL_e = glui_bounds->add_rollout_to_panel(PANEL_panel,_("Truncate data"),false);
    if(chop_rollout!=NULL)*chop_rollout=PANEL_e;

    PANEL_f = glui_bounds->add_panel_to_panel(PANEL_e,"",GLUI_PANEL_NONE);

    *EDIT_con_chopmin = glui_bounds->add_edittext_to_panel(PANEL_f,"",GLUI_EDITTEXT_FLOAT,chopminval,CHOPVALMIN,FILE_CB);
    glui_bounds->add_column_to_panel(PANEL_f,false);

    if(STATIC_con_cmin_unit!=NULL){
      *STATIC_con_cmin_unit=glui_bounds->add_statictext_to_panel(PANEL_f,"xx");
      (*STATIC_con_cmin_unit)->set_w(10);
      glui_bounds->add_column_to_panel(PANEL_f,false);
    }
    *CHECKBOX_con_setchopmin=glui_bounds->add_checkbox_to_panel(PANEL_f,_("Below"),setchopminval,SETCHOPMINVAL,FILE_CB);

    PANEL_h = glui_bounds->add_panel_to_panel(PANEL_e,"",GLUI_PANEL_NONE);

    *EDIT_con_chopmax = glui_bounds->add_edittext_to_panel(PANEL_h,"",GLUI_EDITTEXT_FLOAT,chopmaxval,CHOPVALMAX,FILE_CB);
    glui_bounds->add_column_to_panel(PANEL_h,false);

    if(STATIC_con_cmax_unit!=NULL){
      *STATIC_con_cmax_unit=glui_bounds->add_statictext_to_panel(PANEL_h,"xx");
      glui_bounds->add_column_to_panel(PANEL_h,false);
      (*STATIC_con_cmax_unit)->set_w(10);
    }
    *CHECKBOX_con_setchopmax=glui_bounds->add_checkbox_to_panel(PANEL_h,_("Above"),setchopmaxval,SETCHOPMAXVAL,FILE_CB);

    if(truncatebounds==TRUNCATE_BOUNDS){
      glui_bounds->add_button_to_panel(PANEL_e,_("Update"),CHOPUPDATE,FILE_CB);
    }
  }

}

/* ------------------ PLOT3D_CB ------------------------ */

extern "C" void PLOT3D_CB(int var){
  int i;

  switch(var){
  case UNLOAD_QDATA:
    if(cache_qdata==0){
     PANEL_isosurface->disable();
    }
    else{
      int enable_isosurface;
   
      enable_isosurface=1;
      for(i=0;i<nmeshes;i++){
        mesh *meshi;
        plot3ddata *plot3di;

        meshi = meshinfo + i;
        if(meshi->plot3dfilenum==-1)continue;
        plot3di = plot3dinfo + meshi->plot3dfilenum;
        if(plot3di->loaded==0||plot3di->display==0)continue;
        if(meshi->qdata==NULL){
          enable_isosurface=0;
          break;
        }
      }
      if(enable_isosurface==1)PANEL_isosurface->enable();
      if(enable_isosurface==0)PANEL_isosurface->disable();
    }
    break;
  case PLOT3D_VECTORSKIP:
    if(SPINNER_slicevectorskip!=NULL)SPINNER_slicevectorskip->set_int_val(vectorskip);
    break;
  case UPDATE_VECTOR_FROM_SMV:
    if(SPINNER_vectorpointsize!=NULL&&SPINNER_vectorlinewidth!=NULL&&SPINNER_vectorlinelength!=NULL){
      SPINNER_vectorpointsize->set_float_val(vectorpointsize);
      SPINNER_vectorlinewidth->set_float_val(vectorlinewidth);
      SPINNER_vectorlinelength->set_float_val(vecfactor);
    }
    PLOT3D_CB(UPDATE_VECTOR);
    break;
  case UPDATE_VECTOR:
    updateplotslice(X_SLICE);
    updateplotslice(Y_SLICE);
    updateplotslice(Z_SLICE);
    break;
  case CHOPUPDATE:
    updatechopcolors();
    break;
  case SETCHOPMINVAL:
    updatechopcolors();
    switch(setp3chopmin_temp){
      case 0:
        EDIT_p3_chopmin->disable();
        break;
      case 1:
        EDIT_p3_chopmin->enable();
        break;
      default:
        ASSERT(FFALSE);
        break;
    }
    break;
  case SETCHOPMAXVAL:
    updatechopcolors();
    switch(setp3chopmax_temp){
      case 0:
        EDIT_p3_chopmax->disable();
        break;
      case 1:
        EDIT_p3_chopmax->enable();
        break;
      default:
        ASSERT(FFALSE);
        break;
    }
    break;
  case CHOPVALMIN:
    p3chopmin[list_p3_index]=p3chopmin_temp;
    setp3chopmin[list_p3_index]=setp3chopmin_temp;

    updatechopcolors();
    break;
  case CHOPVALMAX:
    p3chopmax[list_p3_index]=p3chopmax_temp;
    setp3chopmax[list_p3_index]=setp3chopmax_temp;

    updatechopcolors();
    break;
  case PLOTISO:
    visiso = 1 - visiso;
    handleiso();
    glutPostRedisplay();
    break;
  case PLOTISOTYPE:
    updatemenu=1;
    break;
  case UPDATEPLOT:
    UpdateRGBColors(COLORBAR_INDEX_NONE);
    updatemenu=1;
    glutPostRedisplay();
    break;
  case FILETYPEINDEX:
   p3min[list_p3_index_old]=p3min_temp;
   p3max[list_p3_index_old]=p3max_temp;
   setp3min[list_p3_index_old]=setp3min_temp;
   setp3max[list_p3_index_old]=setp3max_temp;
   p3chopmin[list_p3_index_old]=p3chopmin_temp;
   p3chopmax[list_p3_index_old]=p3chopmax_temp;
   setp3chopmin[list_p3_index_old]=setp3chopmin_temp;
   setp3chopmax[list_p3_index_old]=setp3chopmax_temp;

   p3min_temp=p3min[list_p3_index];
   p3max_temp=p3max[list_p3_index];
   setp3min_temp=setp3min[list_p3_index];
   setp3max_temp=setp3max[list_p3_index];
   p3chopmin_temp=p3chopmin[list_p3_index];
   p3chopmax_temp=p3chopmax[list_p3_index];
   setp3chopmin_temp=setp3chopmin[list_p3_index];
   setp3chopmax_temp=setp3chopmax[list_p3_index];
   if(plot3dinfo!=NULL){
     plot3dmin_unit = (unsigned char *)plot3dinfo->label[list_p3_index].unit;
     plot3dmax_unit = plot3dmin_unit;
     update_glui_plot3d_units();
   }

   EDIT_p3_min->set_float_val(p3min_temp);
   EDIT_p3_max->set_float_val(p3max_temp);
   EDIT_p3_chopmin->set_float_val(p3chopmin_temp);
   EDIT_p3_chopmax->set_float_val(p3chopmax_temp);

   list_p3_index_old=list_p3_index;
   RADIO_p3_setmin->set_int_val(setp3min_temp);
   RADIO_p3_setmax->set_int_val(setp3max_temp);
   CHECKBOX_p3_setchopmin->set_int_val(setp3chopmin_temp);
   CHECKBOX_p3_setchopmax->set_int_val(setp3chopmax_temp);
   PLOT3D_CB(SETCHOPMINVAL);
   PLOT3D_CB(SETCHOPMAXVAL);
   PLOT3D_CB(SETVALMIN);
   PLOT3D_CB(SETVALMAX);
   break;
  case SETVALMIN:
   switch(setp3min_temp){
    case PERCENTILE_MIN:
    case GLOBAL_MIN:
      EDIT_p3_min->disable();
      break;
    case SET_MIN:
    case CHOP_MIN:
      EDIT_p3_min->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
   }
   break;
  case SETVALMAX:
     switch(setp3max_temp){
      case PERCENTILE_MIN:
      case GLOBAL_MIN:
        EDIT_p3_max->disable();
        break;
      case SET_MIN:
      case CHOP_MIN:
        EDIT_p3_max->enable();
        break;
      default:
        ASSERT(FFALSE);
        break;
     }
   break;
  case FILEUPDATE:
   p3min[list_p3_index] =    p3min_temp;
   p3max[list_p3_index] =    p3max_temp;
   setp3min[list_p3_index] = setp3min_temp;
   setp3max[list_p3_index] = setp3max_temp;
   break;
  case FILERELOAD:
   PLOT3D_CB(FILEUPDATE);
   for(i=0;i<nplot3dinfo;i++){
     if(plot3dinfo[i].loaded==0)continue;
     LoadPlot3dMenu(i);
   }
   updateglui();
   break;
  case VALMIN:
  case VALMAX:
    break;
  default:
    ASSERT(FFALSE);
    break;
  }
}

/* ------------------ updatetracers ------------------------ */

extern "C" void updatetracers(void){
  if(CHECKBOX_showtracer==NULL)return;
  CHECKBOX_showtracer->set_int_val(show_tracers_always);
}


/* ------------------ update_glui_isotype ------------------------ */

extern "C" void update_glui_isotype(void){
  CHECKBOX_showtrisurface->set_int_val(visAIso&1);
  CHECKBOX_showtrioutline->set_int_val((visAIso&2)/2);
  CHECKBOX_showtripoints->set_int_val((visAIso&4)/4);
}


/* ------------------ update_glui_isotype ------------------------ */

extern "C" void update_glui_plot3dtype(void){
  RADIO_plot3d_isotype->set_int_val(p3dsurfacetype);
}
/* ------------------ updatechar ------------------------ */

extern "C" void updatechar(void){
  if(CHECKBOX_showchar==NULL)return;
  if(canshow_threshold==1){
    CHECKBOX_showchar->enable();
  }
  else{
    CHECKBOX_showchar->disable();
  }
  CHECKBOX_showchar->set_int_val(vis_threshold);
  Bound_CB(SHOWCHAR);
}

/* ------------------ updateplot3dlistindex ------------------------ */

extern "C" void updateplot3dlistindex(void){
  int i;

  if(RADIO_p3==NULL)return;
  i = RADIO_p3->get_int_val();
  if(i!=plotn-1){
    p3min[i]=p3min_temp;
    p3max[i]=p3max_temp;
    setp3min[i]=setp3min_temp;
    setp3max[i]=setp3max_temp;

    p3chopmin[i]=p3chopmin_temp;
    p3chopmax[i]=p3chopmax_temp;
    setp3chopmin[i]=setp3chopmin_temp;
    setp3chopmax[i]=setp3chopmax_temp;
  }

  i=plotn-1;
  list_p3_index_old=i;
  if(i<0)i=0;
  if(i>mxplot3dvars-1)i=mxplot3dvars-1;
  RADIO_p3->set_int_val(i);
  p3min_temp = p3min[i];
  p3max_temp = p3max[i];
  setp3min_temp = setp3min[i];
  setp3max_temp = setp3max[i];

  p3chopmin_temp = p3chopmin[i];
  p3chopmax_temp = p3chopmax[i];
  setp3chopmin_temp = setp3chopmin[i];
  setp3chopmax_temp = setp3chopmax[i];
  if(nplot3dinfo>0){
    PLOT3D_CB(SETVALMIN);
    PLOT3D_CB(SETVALMAX);
    PLOT3D_CB(SETCHOPMINVAL);
    PLOT3D_CB(SETCHOPMAXVAL);
  }
  updatechopcolors();
  updateglui();
}

void Iso_CB(int var){
  switch(var){
  case FRAMELOADING:
    isoframestep_global=isoframeskip_global+1;
    isozipstep=isozipskip+1;
    updatemenu=1;
    break;
  case ISO_SURFACE:
  case  ISO_OUTLINE:
  case ISO_POINTS:
    visAIso= 1*showtrisurface + 2*showtrioutline + 4*showtripoints;
    updatemenu=1;
    break;
  default:
    ASSERT(FFALSE);
    break;
  }
}
void Smoke3D_CB(int var){
  switch(var){
  case FRAMELOADING:
    smoke3dframestep=smoke3dframeskip+1;
    smoke3dzipstep=smoke3dzipskip+1;
    updatemenu=1;
    break;
  default:
    ASSERT(FFALSE);
    break;
  }
}

/* ------------------ add_scriptlist ------------------------ */

extern "C" void add_scriptlist(char *file, int id){
  if(file!=NULL&&strlen(file)>0&&LIST_scriptlist!=NULL){
    LIST_scriptlist->add_item(id,file);
  }
}

/* ------------------ glui_script_enable ------------------------ */

extern "C" void glui_script_enable(void){
    BUTTON_script_start->enable();
    BUTTON_script_stop->enable();
    BUTTON_script_runscript->enable();
    LIST_scriptlist->enable();
    BUTTON_script_saveini->enable();
    BUTTON_script_setsuffix->enable();
    EDIT_ini->enable();
  }

/* ------------------ glui_script_disable ------------------------ */

extern "C"  void glui_script_disable(void){
    BUTTON_script_start->disable();
    BUTTON_script_stop->disable();
    BUTTON_script_runscript->disable();
    LIST_scriptlist->disable();
    BUTTON_script_saveini->disable();
    BUTTON_script_setsuffix->disable();
    EDIT_ini->disable();
  }

  /* ------------------ Script_CB ------------------------ */

  void Script_CB(int var){
    char label[1024];
    char *name;
    int id;
    int len,i;
    int set_renderlabel;

    switch(var){
    case SCRIPT_STEP_NOW:
      keyboard('^',FROM_SMOKEVIEW);
      break;
    case SCRIPT_CANCEL_NOW:
      current_script_command=NULL;
      runscript=0;
      first_frame_index=0;
      skip_render_frames=0;
      script_startframe=-1;
      script_skipframe=-1;
      script_step=0;
      glui_script_enable();
      render_state=RENDER_OFF;
      break;
    case SCRIPT_RENDER_DIR:
      strcpy(label,script_renderdir);
      trim(label);
      name = trim_front(label);
      set_renderlabel=0;
      if(name!=NULL&&strlen(name)!=strlen(script_renderdir)){
        strcpy(script_renderdir,name);
        set_renderlabel=1;
      }
      name=script_renderdir;
      len = strlen(script_renderdir);
      if(len==0)break;
      for(i=0;i<len;i++){
#ifdef WIN32
        if(name[i]=='/'){
          set_renderlabel=1;
          name[i]='\\';
        }
#else
        if(name[i]=='\\'){
          set_renderlabel=1;
          name[i]='/';
        }
#endif
      }
#ifdef WIN32
      if(name[len-1]!='\\'){
        set_renderlabel=1;
        strcat(name,dirseparator);        
      }
#else
      if(name[len-1]!='/'){
        set_renderlabel=1;
        strcat(name,dirseparator);        
      }
#endif
      if(set_renderlabel==1){
        EDIT_renderdir->set_text(script_renderdir);
      }
      break;
    case SCRIPT_RENDER:
      keyboard('r',FROM_SMOKEVIEW);
      break;
    case SCRIPT_RENDER_SUFFIX:
    {
      char *suffix;

      trim(script_renderfilesuffix);
      suffix = trim_front(script_renderfilesuffix);
      strcpy(script_renderfile,"");
      if(strlen(suffix)>0){
        strcpy(script_renderfile,fdsprefix);
        strcat(script_renderfile,"_");
        strcat(script_renderfile,suffix);
        strcpy(label,_("Render: "));
        strcat(label,script_renderfile);
      }
      else{
        strcpy(label,_("Render"));
      }
      BUTTON_script_render->set_name(label);
    }
      break;
    case SCRIPT_START:
      ScriptMenu(SCRIPT_START_RECORDING);
      break;
    case SCRIPT_STOP:
      ScriptMenu(SCRIPT_STOP_RECORDING);
      break;
    case SCRIPT_RUNSCRIPT:
      name = 5+BUTTON_script_runscript->name;
      PRINTF("running script: %s\n",name);
      ScriptMenu(LIST_scriptlist->get_int_val());
      break;
    case SCRIPT_LIST:
      id = LIST_scriptlist->get_int_val();
      name = get_scriptfilename(id);
      if(name!=NULL&&strlen(name)>0){
        strcpy(label,_("Run: "));
        strcat(label,name);
        BUTTON_script_runscript->set_name(label);
      }
      break;
    case SCRIPT_SAVEINI:
      name = 5+BUTTON_script_saveini->name;
      if(strlen(name)>0){
        inifiledata *inifile;

        strcpy(script_filename,name);
        inifile=insert_inifile(name);
        writeini(SCRIPT_INI,script_filename);
        if(inifile!=NULL&&LIST_ini_list!=NULL){
          LIST_ini_list->add_item(inifile->id,inifile->file);
        }
      }
      writeini(LOCAL_INI,NULL);
      break;
    case SCRIPT_LOADINI:
      {
        char *ini_filename;

        id = LIST_ini_list->get_int_val();
        ini_filename = get_inifilename(id);
        if(strcmp(ini_filename,caseini_filename)==0){
          readini(NULL);
        }
        else if(id>=0){
          char *script_filename2;

          if(ini_filename==NULL||strlen(ini_filename)==0)break;
          script_filename2=script_filename;
          strcpy(script_filename,ini_filename);
          windowresized=0;
          readini(script_filename2);
        }
        if(scriptoutstream!=NULL){
          fprintf(scriptoutstream,"LOADINIFILE\n");
          fprintf(scriptoutstream," %s\n",ini_filename);
        }
      }
      break;
    case SCRIPT_STEP:
      update_script_step();
      updatemenu=1;
      break;
    case SCRIPT_FILE_LOADING:
      updatemenu=1;
      break;
    case SCRIPT_EDIT_INI:
      strcpy(label,_("Save "));
      strcat(label,fdsprefix);
      trim(script_inifile_suffix);
      if(strlen(script_inifile_suffix)>0){
        strcat(label,"_");
        strcat(label,script_inifile_suffix);
      }
      strcat(label,".ini");
      BUTTON_script_saveini->set_name(label);
      break;
    case SCRIPT_SETSUFFIX:
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
  }

#ifdef pp_MEMDEBUG

/* ------------------ Memcheck_CB ------------------------ */

  void Memcheck_CB(int var){
  switch(var){
    case MEMCHECK:
      set_memcheck(list_memcheck_index);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
}
#endif

/* ------------------ Bound_CB ------------------------ */

void Bound_CB(int var){
  int i;

  switch(var){
  case SHOWPATCH_BOTH:
    updatefacelists=1;
    updatehiddenfaces=1;
    break;
  case CACHE_BOUNDARYDATA:
    if(cache_boundarydata==0){
      BUTTON_updatebound->disable();
    }
    else{
      BUTTON_updatebound->enable();
    }
    break;
  case VALMAX:
  case VALMIN:
    break;
  case HIDEPATCHSURFACE:
    updatefacelists=1;
    break;
  case FRAMELOADING:
    boundframestep=boundframeskip+1;
    boundzipstep=boundzipskip+1;
    updatemenu=1;
    break;
  case CHOPUPDATE:
    updatechopcolors();
    break;
  case SETCHOPMINVAL:
    updatechopcolors();
    local2globalpatchbounds(patchlabellist[list_patch_index]);
    switch(setpatchchopmin){
      case 0:
      EDIT_patch_chopmin->disable();
      break;
      case 1:
      EDIT_patch_chopmin->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    update_hidepatchsurface();
    break;
  case SETCHOPMAXVAL:
    updatechopcolors();
    local2globalpatchbounds(patchlabellist[list_patch_index]);
    switch(setpatchchopmax){
      case 0:
        EDIT_patch_chopmax->disable();
        break;
      case 1:
        EDIT_patch_chopmax->enable();
        break;
      default:
        ASSERT(FFALSE);
        break;
    }
    update_hidepatchsurface();
    break;
  case CHOPVALMIN:
    ASSERT(EDIT_patch_min!=NULL);
    EDIT_patch_min->set_float_val(patchmin);
    local2globalpatchbounds(patchlabellist[list_patch_index]);
    updatechopcolors();
    break;
  case CHOPVALMAX:
    ASSERT(EDIT_patch_max!=NULL);
    EDIT_patch_max->set_float_val(patchmax);
    local2globalpatchbounds(patchlabellist[list_patch_index]);
    updatechopcolors();
    break;
  case SHOWCHAR:
    if(CHECKBOX_showchar!=NULL&&CHECKBOX_showonlychar!=NULL){
      if(vis_threshold==1){
        CHECKBOX_showonlychar->enable();
      }
      else{
        CHECKBOX_showonlychar->disable();
      }
    }
    updatemenu=1;
    updatefacelists=1;
    break;
  case FILETYPEINDEX:
    local2globalpatchbounds(patchlabellist[list_patch_index_old]);
    global2localpatchbounds(patchlabellist[list_patch_index]);

    EDIT_patch_min->set_float_val(patchmin);
    EDIT_patch_max->set_float_val(patchmax);
    EDIT_patch_chopmin->set_float_val(patchchopmin);
    EDIT_patch_chopmax->set_float_val(patchchopmax);

    Bound_CB(SETVALMIN);
    Bound_CB(SETVALMAX);
    if(RADIO_patch_setmin!=NULL)RADIO_patch_setmin->set_int_val(setpatchmin);
    if(RADIO_patch_setmax!=NULL)RADIO_patch_setmax->set_int_val(setpatchmax);
    if(CHECKBOX_patch_setchopmin!=NULL)CHECKBOX_patch_setchopmin->set_int_val(setpatchchopmin);
    if(CHECKBOX_patch_setchopmax!=NULL)CHECKBOX_patch_setchopmax->set_int_val(setpatchchopmax);

    switch(setpatchchopmin){
      case 0:
        EDIT_patch_chopmin->disable();
        break;
      case 1:
        EDIT_patch_chopmin->enable();
        break;
      default:
        ASSERT(FFALSE);
        break;
    }
    switch(setpatchchopmax){
      case 0:
        EDIT_patch_chopmax->disable();
        break;
      case 1:
        EDIT_patch_chopmax->enable();
        break;
      default:
        ASSERT(FFALSE);
        break;
    }

    list_patch_index_old = list_patch_index;
    update_hidepatchsurface();
    break;
  case SETVALMIN:
    switch(setpatchmin){
    case PERCENTILE_MIN:
    case GLOBAL_MIN:
      EDIT_patch_min->disable();
      break;
    case SET_MIN:
      EDIT_patch_min->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    Bound_CB(FILEUPDATE);
    break;
  case SETVALMAX:
    switch(setpatchmax){
    case PERCENTILE_MAX:
    case GLOBAL_MAX:
      EDIT_patch_max->disable();
      break;
    case SET_MAX:
      EDIT_patch_max->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    Bound_CB(FILEUPDATE);
    break;
  case FILEUPDATE:
    local2globalpatchbounds(patchlabellist[list_patch_index]);
    break;
  case FILEUPDATEDATA:
    UpdateAllPatchColors();
    break;
  case FILERELOAD:
    Bound_CB(FILEUPDATE);
    for(i=0;i<npatchinfo;i++){
      patchdata *patchi;

      patchi=patchinfo+i;
      if(patchi->loaded==0)continue;
      LoadPatchMenu(i);
    }
    EDIT_patch_min->set_float_val(patchmin);
    EDIT_patch_max->set_float_val(patchmax);
    break;
  case COMPRESS_FILES:
    compress_svzip();
    break;
  case COMPRESS_AUTOLOADED:
    updatemenu=1;
    break;
  case OVERWRITE:
    if(overwrite_all==1){
      CHECKBOX_erase_all->set_int_val(0);
    }
    updatemenu=1;
    break;
  case ERASE:
    if(erase_all==1){
      CHECKBOX_overwrite_all->set_int_val(0);
    }
    updatemenu=1;
    break;
  case STARTUP:
    Bounds_DLG_CB(SAVE_SETTINGS);
    break;
  case SAVE_FILE_LIST:
    set_3dsmoke_startup();
    Bounds_DLG_CB(SAVE_SETTINGS);
    break;
  case LOAD_FILES:
      load_Files();
    break;
  default:
    ASSERT(FFALSE);
    break;
  }
}

/* ------------------ updatepatchlistindex ------------------------ */

extern "C" void updatepatchlistindex(int patchfilenum){
  int i;
  if(RADIO_bf==NULL)return;
  for(i=0;i<npatch2;i++){
    patchdata *patchi;

    patchi = patchinfo + patchfilenum;
    if(strcmp(patchlabellist[i],patchi->label.shortlabel)==0){
      RADIO_bf->set_int_val(i);
      list_patch_index_old=list_patch_index;
      global2localpatchbounds(patchlabellist[i]);
      RADIO_patch_setmin->set_int_val(setpatchmin);
      RADIO_patch_setmax->set_int_val(setpatchmax);
      EDIT_patch_min->set_float_val(patchmin);
      EDIT_patch_max->set_float_val(patchmax);

      CHECKBOX_patch_setchopmin->set_int_val(setpatchchopmin);
      CHECKBOX_patch_setchopmax->set_int_val(setpatchchopmax);
      EDIT_patch_chopmin->set_float_val(patchchopmin);
      EDIT_patch_chopmax->set_float_val(patchchopmax);

      if(setpatchmin==SET_MIN){
        EDIT_patch_min->enable();
      }
      else{
        EDIT_patch_min->disable();
      }
      if(setpatchmax==SET_MAX){
        EDIT_patch_max->enable();
      }
      else{
        EDIT_patch_max->disable();
      }

      if(setpatchchopmin==SET_MIN){
        EDIT_patch_chopmin->enable();
      }
      else{
        EDIT_patch_chopmin->disable();
      }
      if(setpatchchopmax==SET_MAX){
        EDIT_patch_chopmax->enable();
      }
      else{
        EDIT_patch_chopmax->disable();
      }
      return;
    }
  }
}

/* ------------------ updatepatchlistindex2 ------------------------ */

extern "C" void updatepatchlistindex2(char *label){
  int i;
  for(i=0;i<npatch2;i++){
    if(strcmp(patchlabellist[i],label)==0){
      updatepatchlistindex(patchlabellist_index[i]);
      break;
    }
  }
}

/* ------------------ update_glui_streakvalue ------------------------ */

extern "C" void update_glui_streakvalue(float rvalue){
  float_streak5value=rvalue;
  if(SPINNER_partstreaklength!=NULL){
    SPINNER_partstreaklength->set_float_val(rvalue);
    SPINNER_partstreaklength->set_float_limits(0.0,tmax_part);
  }
}

/* ------------------ PART_CB ------------------------ */

void PART_CB(int var){
  part5prop *prop_new, *prop_old;

  prop_new = part5propinfo + ipart5prop;
  prop_old = part5propinfo + ipart5prop_old;
  switch(var){
  case VALMIN:
    if(setpartmax==SET_MAX)prop_new->user_max=partmax;
    break;
  case VALMAX:
    if(setpartmin==SET_MIN)prop_new->user_min=partmin;
    break;
  case FILETYPEINDEX:

    // save data from controls

    prop_old->setvalmin=setpartmin;
    prop_old->setvalmax=setpartmax;
    if(setpartmin==SET_MIN){
      prop_old->user_min=partmin;
    }
    if(setpartmax==SET_MAX){
      prop_old->user_max=partmax;
    }
    prop_old->setchopmin=setpartchopmin;
    prop_old->setchopmax=setpartchopmax;
    prop_old->chopmin=partchopmin;
    prop_old->chopmax=partchopmax;

    // copy data to controls

    setpartmin=prop_new->setvalmin;
    setpartmax=prop_new->setvalmax;
    PART_CB(SETVALMIN);
    PART_CB(SETVALMAX);

    setpartchopmin=prop_new->setchopmin;
    setpartchopmax=prop_new->setchopmax;
    partchopmin=prop_new->chopmin;
    partchopmax=prop_new->chopmax;

    partmin_unit = (unsigned char *)prop_new->label->unit;
    partmax_unit = partmin_unit;
    update_glui_part_units();

    // update controls

    if(RADIO_part_setmin!=NULL)RADIO_part_setmin->set_int_val(setpartmin);
    if(RADIO_part_setmax!=NULL)RADIO_part_setmax->set_int_val(setpartmax);


    if(EDIT_part_chopmin!=NULL)EDIT_part_chopmin->set_float_val(partchopmin);
    if(EDIT_part_chopmax!=NULL)EDIT_part_chopmax->set_float_val(partchopmax);
    if(CHECKBOX_part_setchopmin!=NULL)CHECKBOX_part_setchopmin->set_int_val(setpartchopmin);
    if(CHECKBOX_part_setchopmax!=NULL)CHECKBOX_part_setchopmax->set_int_val(setpartchopmax);

    ipart5prop_old = ipart5prop;

    if(CHECKBOX_part_setchopmin!=NULL)PART_CB(SETCHOPMINVAL);
    if(CHECKBOX_part_setchopmax!=NULL)PART_CB(SETCHOPMAXVAL);

    break;
  case STREAKLENGTH:
    update_streakvalue(float_streak5value-0.001);
    if(float_streak5value==0.0){
      streak5show=0;
    }
    else{
      streak5show=1;
    }
    updatemenu=1;
    break;
  case TRACERS:
    updatemenu=1;
    break;
  case FRAMELOADING:
    partframestep=partframeskip+1;
    partpointstep=partpointskip+1;
    evacframestep=evacframeskip+1;
    evacframestep=evacframeskip+1;
    if(partpointstep!=partpointstep_old){
      update_all_partvis2();
      partpointstep_old=partpointstep;
    }

    updatemenu=1;
    break;
  case CHOPUPDATE:
    updatechopcolors();
    break;
  case SETCHOPMINVAL:
    prop_new->setchopmin=setpartchopmin;
    prop_new->chopmin=partchopmin;
    updatechopcolors();
    switch(setpartchopmin){
      case 0:
      EDIT_part_chopmin->disable();
      break;
      case 1:
      EDIT_part_chopmin->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case SETCHOPMAXVAL:
    prop_new->setchopmax=setpartchopmax;
    prop_new->chopmax=partchopmax;
    updatechopcolors();
    switch(setpartchopmax){
      case 0:
      EDIT_part_chopmax->disable();
      break;
      case 1:
      EDIT_part_chopmax->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case CHOPVALMIN:
    prop_new->setchopmin=setpartchopmin;
    prop_new->chopmin=partchopmin;
    if(EDIT_part_chopmin!=NULL)EDIT_part_chopmin->set_float_val(partchopmin);
    updatechopcolors();
    break;
  case CHOPVALMAX:
    prop_new->setchopmax=setpartchopmax;
    prop_new->chopmax=partchopmax;
    if(EDIT_part_chopmax!=NULL)EDIT_part_chopmax->set_float_val(partchopmax);
    updatechopcolors();
    break;
  case SETVALMIN:
    if(setpartmin_old==SET_MIN){
      if(prop_old!=NULL)prop_old->user_min=partmin;
    }
    setpartmin_old=setpartmin;
    switch(setpartmin){
    case PERCENTILE_MIN:
      if(prop_new!=NULL)partmin=prop_new->percentile_min;
      if(EDIT_part_min!=NULL)EDIT_part_min->disable();
      break;
    case GLOBAL_MIN:
      if(prop_new!=NULL)partmin=prop_new->global_min;
      if(EDIT_part_min!=NULL)EDIT_part_min->disable();
      break;
    case SET_MIN:
      if(prop_new!=NULL)partmin=prop_new->user_min;
      if(EDIT_part_min!=NULL)EDIT_part_min->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    if(prop_new!=NULL)prop_new->valmin=partmin;
    if(EDIT_part_min!=NULL)EDIT_part_min->set_float_val(partmin);
   break;
  case SETVALMAX:
    if(setpartmax_old==SET_MAX){
      if(prop_old!=NULL)prop_old->user_max=partmax;
    }
    setpartmax_old=setpartmax;
    switch(setpartmax){
    case PERCENTILE_MAX:
      if(prop_new!=NULL)partmax=prop_new->percentile_max;
      if(EDIT_part_max!=NULL)EDIT_part_max->disable();
      break;
    case GLOBAL_MAX:
      if(prop_new!=NULL)partmax=prop_new->global_max;
      if(EDIT_part_max!=NULL)EDIT_part_max->disable();
      break;
    case SET_MAX:
      if(prop_new!=NULL)partmax=prop_new->user_max;
      if(EDIT_part_max!=NULL)EDIT_part_max->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    if(prop_new!=NULL)prop_new->valmax=partmax;
    if(EDIT_part_max!=NULL)EDIT_part_max->set_float_val(partmax);
   break;
  case FILERELOAD:
    {
      int prop_index_SAVE;

     prop_index_SAVE=prop_index;
     PART_CB(FILETYPEINDEX);
     if(EDIT_part_min!=NULL&&setpartmin==SET_MIN)PART_CB(SETVALMIN);
     if(EDIT_part_max!=NULL&&setpartmax==SET_MAX)PART_CB(SETVALMAX);
     ParticleMenu(0);
     updateglui();
     ParticlePropShowMenu(prop_index_SAVE);
    }
    break;
  default:
    ASSERT(FFALSE);
    break;
  }
}

/* ------------------ Time_CB ------------------------ */

void Time_CB(int var){

  updatemenu=1;
  switch(var){
  case SET_TIME:
    settimeval(glui_time);
    break;
  case TBOUNDS:
    if(use_tload_begin==1||use_tload_end==1||use_tload_skip==1){
      update_tbounds();
    }
    break;
  case TBOUNDS_USE:
    if(use_tload_begin==1){
      SPINNER_tload_begin->enable();
    }
    else{
      SPINNER_tload_begin->disable();
    }
    if(use_tload_end==1){
      SPINNER_tload_end->enable();
    }
    else{
      SPINNER_tload_end->disable();
    }
    if(use_tload_skip==1){
      SPINNER_tload_skip->enable();
    }
    else{
      SPINNER_tload_skip->disable();
    }
    update_tbounds();
    break;
  case RELOAD_DATA:
    ReloadMenu(RELOAD_NOW);
    break;
  default:
    ASSERT(FFALSE);
    break;
  }
}

/* ------------------ SLICE_CB ------------------------ */

extern "C" void Slice_CB(int var){
  int error,i;
  int ii;
  slicedata *sd;

  updatemenu=1;
  if(var==DATA_transparent){
    if(CHECKBOX_transparentflag2!=NULL)CHECKBOX_transparentflag2->set_int_val(use_transparency_data);
    updatechopcolors();
    return;
  }

  if(var==COLORBAR_EXTREME2){
    update_extreme();
    return;
  }
  if(var==COLORBAR_LIST2){
      selectedcolorbar_index=get_colorbar_list_index();
      update_colorbar_list();
      ColorBarMenu(selectedcolorbar_index);
      colorbar_global2local();
  }
  if(var==COLORBAR_SMOOTH){
    updatemenu=1;
    return;
  }
  switch(var){
    case SLICE_VECTORSKIP:
      if(SPINNER_plot3dvectorskip!=NULL)SPINNER_plot3dvectorskip->set_int_val(vectorskip);
      break;
    case ZONEVALMIN:
      getZoneColors(zonetu, nzonetotal, izonetu,zonemin, zonemax, nrgb, nrgb_full, 
        colorlabelzone, zonescale, zonelevels256);
      zoneusermin=zonemin;
      break;
    case ZONEVALMAX:
      getZoneColors(zonetu, nzonetotal, izonetu,zonemin, zonemax, nrgb, nrgb_full, 
        colorlabelzone, zonescale, zonelevels256);
      zoneusermax=zonemax;
      break;
    case SETZONEVALMIN:
      if(setzonemin==SET_MIN){
        EDIT_zone_min->enable();
        zonemin=zoneusermin;
        EDIT_zone_min->set_float_val(zonemin);
      }
      else{
        EDIT_zone_min->disable();
        EDIT_zone_min->set_float_val(zoneglobalmin);
      }
      getZoneColors(zonetu, nzonetotal, izonetu,zonemin, zonemax, nrgb, nrgb_full, 
        colorlabelzone, zonescale, zonelevels256);
      break;
    case SETZONEVALMAX:
      if(setzonemax==SET_MAX){
        EDIT_zone_max->enable();
        zonemax=zoneusermax;
        EDIT_zone_max->set_float_val(zonemax);
      }
      else{
        EDIT_zone_max->disable();
        EDIT_zone_max->set_float_val(zoneglobalmax);
      }
      getZoneColors(zonetu, nzonetotal, izonetu,zonemin, zonemax, nrgb, nrgb_full, 
        colorlabelzone, zonescale, zonelevels256);
      break;
    case COLORBAR_LIST2:
      break;
    case RESEARCH_MODE:
      for(i=0;i<nsliceinfo;i++){
        slicedata *slicei;

        slicei = sliceinfo + i;
        if(slicei->loaded==0||slicei->display==0)continue;
        updateslicelist(getslicetype(slicei));
        break;
      }
      if(research_mode==1){
        axislabels_smooth_save=axislabels_smooth;
        axislabels_smooth=0;
        visColorbarLabels_save=visColorbarLabels;
        visColorbarLabels=1;
        
        setslicemin_save=setslicemin;
        slicemin_save=slicemin;
        setslicemin=GLOBAL_MIN;
        Slice_CB(SETVALMIN);
        
        setslicemax_save=setslicemax;
        slicemax_save=slicemax;
        setslicemax=GLOBAL_MAX;
        Slice_CB(SETVALMAX);
        PRINTF("resarch mode on\n");
      }
      else{
        axislabels_smooth=axislabels_smooth_save;
        visColorbarLabels=visColorbarLabels_save;

        setslicemin=setslicemin_save;
        Slice_CB(SETVALMIN);
        slicemin=slicemin_save;
        Slice_CB(VALMIN);
        
        setslicemax=setslicemax_save;
        Slice_CB(SETVALMAX);
        slicemax=slicemax_save;
        Slice_CB(VALMAX);
        PRINTF("resarch mode off\n");
      }
      update_axislabels_smooth();
      Slice_CB(FILEUPDATE);
      break;
#ifdef pp_BETA
    case SMOOTH_SURFACES:
      CHECKBOX_smooth2->set_int_val(smoothtrinormal);
      break;
    case SORT_SURFACES:
      sort_embedded_geometry=sort_iso_triangles;
      for(i=nsurfinfo;i<nsurfinfo+n_iso_ambient+1;i++){
        surfdata *surfi;

        surfi = surfinfo + i;
        surfi->transparent_level=transparent_level;
      }
      CHECKBOX_sort2->set_int_val(sort_iso_triangles);
      break;
#endif
    case SHOW_EVAC_SLICES:
      data_evac_coloring = 1-constant_evac_coloring;
      update_slice_menu_show();
      if(CHECKBOX_data_coloring!=NULL)CHECKBOX_data_coloring->set_int_val(data_evac_coloring);
      break;
    case DATA_EVAC_COLORING:
      constant_evac_coloring = 1-data_evac_coloring;
      update_slice_menu_show();
      if(CHECKBOX_constant_coloring!=NULL)CHECKBOX_constant_coloring->set_int_val(constant_evac_coloring);
      break;
    case COLORBAND:
      UpdateRGBColors(colorbar_select_index);
      break;
    case TRANSPARENTLEVEL:
      for(i=nsurfinfo;i<nsurfinfo+n_iso_ambient+1;i++){
        surfdata *surfi;

        surfi = surfinfo + i;
        surfi->transparent_level=transparent_level;
      }
      UpdateRGBColors(COLORBAR_INDEX_NONE);
      if(SPINNER_transparent_level!=NULL)SPINNER_transparent_level->set_float_val(transparent_level);
      if(SPINNER_labels_transparency_data2!=NULL)SPINNER_labels_transparency_data2->set_float_val(transparent_level);
      break;
    case LINE_CONTOUR_VALUE:
      if(slice_line_contour_num<1){
        slice_line_contour_num=1;
        SPINNER_line_contour_num->set_int_val(slice_line_contour_num);
      }
      if(slice_line_contour_num==1&&slice_line_contour_min!=slice_line_contour_max){
        slice_line_contour_max=slice_line_contour_min;
        SPINNER_line_contour_max->set_float_val(slice_line_contour_max);
      }
      slicebounds[list_slice_index].line_contour_min=slice_line_contour_min;
      slicebounds[list_slice_index].line_contour_max=slice_line_contour_max;
      slicebounds[list_slice_index].line_contour_num=slice_line_contour_num;
      break;
    case UPDATE_LINE_CONTOUR_VALUE:
      update_slice_contours(list_slice_index,slice_line_contour_min, slice_line_contour_max,slice_line_contour_num);
      break;
  case UPDATE_VECTOR_FROM_SMV:
    if(SPINNER_plot3d_vectorpointsize!=NULL&&SPINNER_plot3d_vectorlinewidth!=NULL&&SPINNER_plot3d_vectorlinelength!=NULL){
      SPINNER_plot3d_vectorpointsize->set_float_val(vectorpointsize);
      SPINNER_plot3d_vectorlinewidth->set_float_val(vectorlinewidth);
      SPINNER_plot3d_vectorlinelength->set_float_val(vecfactor);
    }
    Slice_CB(UPDATE_VECTOR);
    break;
  case UPDATE_VECTOR:
    break;
  case FRAMELOADING:
    sliceframestep=sliceframeskip+1;
    slicezipstep=slicezipskip+1;
    updatemenu=1;
    break;
  case CHOPUPDATE:
    updatechopcolors();
    break;
  case SETCHOPMINVAL:
    updatechopcolors();
    SETslicemin(setslicemin,slicemin,setslicechopmin,slicechopmin);
    switch(setslicechopmin){
      case 0:
        EDIT_slice_chopmin->disable();
        break;
      case 1:
        EDIT_slice_chopmin->enable();
        break;
      default:
        ASSERT(FFALSE);
        break;
    }
    break;
  case SETCHOPMAXVAL:
    updatechopcolors();
    SETslicemax(setslicemax,slicemax,setslicechopmax,slicechopmax);
    switch(setslicechopmax){
      case 0:
      EDIT_slice_chopmax->disable();
      break;
      case 1:
      EDIT_slice_chopmax->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case CHOPVALMIN:
    if(EDIT_slice_min!=NULL)EDIT_slice_min->set_float_val(slicemin);
    SETslicemin(setslicemin,slicemin,setslicechopmin,slicechopmin);
    updatechopcolors();
    break;
  case CHOPVALMAX:
    if(EDIT_slice_max!=NULL)EDIT_slice_max->set_float_val(slicemax);
    SETslicemax(setslicemax,slicemax,setslicechopmax,slicechopmax);
    updatechopcolors();
    break;
  case SETVALMIN:
    switch(setslicemin){
    case PERCENTILE_MIN:
    case GLOBAL_MIN:
      if(EDIT_slice_min!=NULL)EDIT_slice_min->disable();
      break;
    case SET_MIN:
      if(EDIT_slice_min!=NULL)EDIT_slice_min->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    if(RADIO_slice_setmin!=NULL)RADIO_slice_setmin->set_int_val(setslicemin);
    SETslicemin(setslicemin,slicemin,setslicechopmin,slicechopmin);
    break;
  case SETVALMAX:
    switch(setslicemax){
      case PERCENTILE_MAX:
      case GLOBAL_MAX:
        if(EDIT_slice_max!=NULL)EDIT_slice_max->disable();
        break;
      case SET_MAX:
        if(EDIT_slice_max!=NULL)EDIT_slice_max->enable();
        break;
      default:
        ASSERT(FFALSE);
        break;
    }
    if(RADIO_slice_setmax!=NULL)RADIO_slice_setmax->set_int_val(setslicemax);
    SETslicemax(setslicemax,slicemax,setslicechopmax,slicechopmax);
    break;
  case VALMIN:
    if(EDIT_slice_min!=NULL)EDIT_slice_min->set_float_val(slicemin);
    SETslicemin(setslicemin,slicemin,setslicechopmin,slicechopmin);
    break;
  case VALMAX:
    if(EDIT_slice_max!=NULL)EDIT_slice_max->set_float_val(slicemax);
    SETslicemax(setslicemax,slicemax,setslicechopmax,slicechopmax);
    break;
  case FILETYPEINDEX:
    if(slice_bounds_dialog==1&&list_slice_index==fire_line_index){
      slice_bounds_dialog=0;
      if(ROLLOUT_slice_bound!=NULL){
        ROLLOUT_slice_bound->close();
        ROLLOUT_slice_bound->disable();
      }
      if(ROLLOUT_slice_chop!=NULL){
        ROLLOUT_slice_chop->close();
        ROLLOUT_slice_chop->disable();
      }
    }
    if(slice_bounds_dialog==0&&list_slice_index!=fire_line_index){
      slice_bounds_dialog=1;
      if(ROLLOUT_slice_bound!=NULL){
        ROLLOUT_slice_bound->enable();
      }
      if(ROLLOUT_slice_chop!=NULL){
        ROLLOUT_slice_chop->enable();
      }
    }
    setslicebounds(list_slice_index);
    if(EDIT_slice_min!=NULL)EDIT_slice_min->set_float_val(slicemin);
    switch(setslicemin){
    case PERCENTILE_MIN:
    case GLOBAL_MIN:
      if(EDIT_slice_min!=NULL)EDIT_slice_min->disable();
      break;
    case SET_MIN:
      if(EDIT_slice_min!=NULL)EDIT_slice_min->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    if(EDIT_slice_max!=NULL)EDIT_slice_max->set_float_val(slicemax);
    switch(setslicemax){
    case PERCENTILE_MIN:
    case GLOBAL_MAX:
      if(EDIT_slice_max!=NULL)EDIT_slice_max->disable();
      break;
    case SET_MAX:
      if(EDIT_slice_max!=NULL)EDIT_slice_max->enable();
      break;
    default:
      ASSERT(FFALSE);
      break;
    }

    RADIO_slice_setmin->set_int_val(setslicemin);
    RADIO_slice_setmax->set_int_val(setslicemax);

    EDIT_slice_chopmin->set_float_val(slicechopmin);
    EDIT_slice_chopmax->set_float_val(slicechopmax);
    CHECKBOX_slice_setchopmin->set_int_val(setslicechopmin);
    CHECKBOX_slice_setchopmax->set_int_val(setslicechopmax);

    if(setslicechopmin==1){
      EDIT_slice_chopmin->enable();
    }
    else{
      EDIT_slice_chopmin->disable();
    }
    if(setslicechopmax==1){
      EDIT_slice_chopmax->enable();
    }
    else{
      EDIT_slice_chopmax->disable();
    }
    SPINNER_line_contour_min->set_float_val(slice_line_contour_min);
    SPINNER_line_contour_max->set_float_val(slice_line_contour_max);
    SPINNER_line_contour_num->set_int_val(slice_line_contour_num);
    break;
  case FILEUPDATE:
    for(ii=0;ii<nslice_loaded;ii++){
      i = slice_loaded_list[ii];
      sd = sliceinfo + i;
      if(sd->type!=islicetype)continue;
      readslice("",i, RESETBOUNDS, &error);
    }
    break;
  case FILERELOAD:
    Slice_CB(FILEUPDATE);
    if(slicefilenum>=0&&slicefilenum<nsliceinfo){
      LoadSliceMenu(slicefilenum);
    }
    else{
      LoadSliceMenu(0);
    }
    updateglui();
    break;
  default:
    ASSERT(FFALSE);
    break;
  }
}

/* ------------------ SETslicemin ------------------------ */

void SETslicemin(int setslicemin_local, float slicemin_local,int setslicechopmin_local, float slicechopmin_local){
  if(slicebounds==NULL)return;
  slicebounds[list_slice_index].setvalmin=setslicemin_local;
  slicebounds[list_slice_index].valmin=slicemin_local;
  slicebounds[list_slice_index].setchopmin=setslicechopmin_local;
  slicebounds[list_slice_index].chopmin=slicechopmin_local;
}

/* ------------------ SETslicemax ------------------------ */

void SETslicemax(int setslicemax_local, float slicemax_local,int setslicechopmax_local, float slicechopmax_local){
  if(slicebounds==NULL)return;
  slicebounds[list_slice_index].setvalmax=setslicemax_local;
  slicebounds[list_slice_index].valmax=slicemax_local;
  slicebounds[list_slice_index].setchopmax=setslicechopmax_local;
  slicebounds[list_slice_index].chopmax=slicechopmax_local;
}

/* ------------------ updateslicelist ------------------------ */

extern "C" void updateslicelist(int index){
  RADIO_slice->set_int_val(index);
}

/* ------------------ updateslicelistindex ------------------------ */

extern "C" void updateslicelistindex(int sfn){
  int i;
  int slicefiletype;
  slicedata *sd;
  if(sfn<0){
    updateslicefilenum();
    sfn=slicefilenum;
  }
  sd = sliceinfo+sfn;
  slicefiletype=getsliceindex(sd);
  if(slicefiletype>=0&&slicefiletype<nslice2){
    i = slicefiletype;
    RADIO_slice->set_int_val(i);
    setslicebounds(i);
    list_slice_index=i;
    Slice_CB(SETVALMIN);
    Slice_CB(SETVALMAX);
    Slice_CB(VALMIN);
    Slice_CB(VALMAX);
    Slice_CB(SETCHOPMINVAL);
    Slice_CB(SETCHOPMAXVAL);
    Slice_CB(CHOPVALMIN);
    Slice_CB(CHOPVALMAX);
  }
}

/* ------------------ updateglui ------------------------ */

extern "C" void updateglui(void){
  GLUI_Master.sync_live_all();
}

/* ------------------ Bounds_DLG_CB ------------------------ */

void Bounds_DLG_CB(int var){
  switch(var){
  case CLOSE:
    glui_bounds->hide();
    showbounds_dialog=0;
    updatemenu=1;
    break;
  case SAVE_SETTINGS:
    writeini(LOCAL_INI,NULL);
    break;
  case COMPRESS_FILES:
    PRINTF("compressing\n");
    break;
  default:
    ASSERT(FFALSE);
    break;
  }
}

/* ------------------ show_glui ------------------------ */

extern "C" void show_glui_bounds(int menu_id){
  int islice, ipatch;
    if(toggle_dialogs==1){
      if(ROLLOUT_boundary!=NULL)ROLLOUT_boundary->close();
      if(ROLLOUT_plot3d != NULL)ROLLOUT_plot3d->close();
      if(ROLLOUT_evac != NULL)ROLLOUT_evac->close();
      if(ROLLOUT_part != NULL)ROLLOUT_part->close();
      if(ROLLOUT_slice != NULL)ROLLOUT_slice->close();
      if(ROLLOUT_bound != NULL)ROLLOUT_bound->close();
      if(ROLLOUT_iso != NULL)ROLLOUT_iso->close();
      if(ROLLOUT_volsmoke3d != NULL)ROLLOUT_volsmoke3d->close();
      if(ROLLOUT_time != NULL)ROLLOUT_time->close();
      if(ROLLOUT_colorbar != NULL)ROLLOUT_colorbar->close();
      if(ROLLOUT_outputpatchdata != NULL)ROLLOUT_outputpatchdata->close();
    }
    
  showbounds_dialog=0;
  showscript_dialog=0;
  showconfig_dialog = 0;
  showzip_dialog = 0;
  show3dsmoke_dialog=0;
  showfiles_dialog = 0;
  showload_dialog = 0;
  if(menu_id==DIALOG_BOUNDS){
    showbounds_dialog=1;
    if(nsliceinfo>0){
      islice=RADIO_slice->get_int_val();
      setslicebounds(islice);
      Slice_CB(SETVALMIN);
      Slice_CB(SETVALMAX);
      Slice_CB(VALMIN);
      Slice_CB(VALMAX);
    }
    if(npatchinfo>0){
      ipatch=RADIO_bf->get_int_val();
      global2localpatchbounds(patchlabellist[ipatch]);
      Bound_CB(SETVALMIN);
      Bound_CB(SETVALMAX);
    }
    if(npartinfo>0&&npartinfo!=nevac){
      PART_CB(SETVALMIN);
      PART_CB(SETVALMAX);
    }
    if(nplot3dinfo>0){
      PLOT3D_CB(SETVALMIN);
      PLOT3D_CB(SETVALMAX);
    }

    if(nsliceinfo>0||npatchinfo>0)updateglui();

    updatechar();
    if(ROLLOUT_filebounds!=NULL)ROLLOUT_filebounds->open();
    if(toggle_dialogs==1){
      if(ROLLOUT_time!=NULL)ROLLOUT_time->close();
      if(ROLLOUT_autoload!=NULL)ROLLOUT_autoload->close();
      if(ROLLOUT_showhide != NULL)ROLLOUT_showhide->close();
      if(ROLLOUT_script != NULL)ROLLOUT_script->close();
      if(ROLLOUT_config!=NULL)ROLLOUT_config->close();
      if(ROLLOUT_compress!=NULL)ROLLOUT_compress->close();
      if(ROLLOUT_smoke3d != NULL)ROLLOUT_smoke3d->close();
    }
  }
  else if(menu_id == DIALOG_SHOWFILES){
    showfiles_dialog = 1;
    if(ROLLOUT_showhide != NULL)ROLLOUT_showhide->open();
    if(toggle_dialogs==1){
      if(ROLLOUT_time!=NULL)ROLLOUT_time->close();
      if(ROLLOUT_autoload!=NULL)ROLLOUT_autoload->close();
      if(ROLLOUT_filebounds!=NULL)ROLLOUT_filebounds->close();
      if(ROLLOUT_script != NULL)ROLLOUT_script->close();
      if(ROLLOUT_config!=NULL)ROLLOUT_config->close();
      if(ROLLOUT_compress!=NULL)ROLLOUT_compress->close();
      if(ROLLOUT_smoke3d != NULL)ROLLOUT_smoke3d->close();
    }
  }
  else if(menu_id==DIALOG_CONFIG){
    showconfig_dialog = 1;
    if(ROLLOUT_config!=NULL)ROLLOUT_config->open();
    if(toggle_dialogs==1){
      if(ROLLOUT_time!=NULL)ROLLOUT_time->close();
      if(ROLLOUT_autoload!=NULL)ROLLOUT_autoload->close();
      if(ROLLOUT_filebounds!=NULL)ROLLOUT_filebounds->close();
      if(ROLLOUT_showhide!=NULL)ROLLOUT_showhide->close();
      if(ROLLOUT_script!=NULL)ROLLOUT_script->close();
      if(ROLLOUT_compress!=NULL)ROLLOUT_compress->close();
      if(ROLLOUT_smoke3d!=NULL)ROLLOUT_smoke3d->close();
    }
  }
  else if(menu_id==DIALOG_AUTOLOAD){
    showload_dialog = 1;
    if(ROLLOUT_autoload!=NULL)ROLLOUT_autoload->open();
    if(toggle_dialogs==1){
      if(ROLLOUT_time!=NULL)ROLLOUT_time->close();
      if(ROLLOUT_filebounds!=NULL)ROLLOUT_filebounds->close();
      if(ROLLOUT_showhide!=NULL)ROLLOUT_showhide->close();
      if(ROLLOUT_script!=NULL)ROLLOUT_script->close();
      if(ROLLOUT_config!=NULL)ROLLOUT_config->close();
      if(ROLLOUT_compress!=NULL)ROLLOUT_compress->close();
      if(ROLLOUT_smoke3d!=NULL)ROLLOUT_smoke3d->close();
    }
  }
  else if(menu_id==DIALOG_TIME){
    showtime_dialog = 1;
    if(ROLLOUT_time!=NULL)ROLLOUT_time->open();
    if(toggle_dialogs==1){
      if(ROLLOUT_autoload!=NULL)ROLLOUT_autoload->close();
      if(ROLLOUT_filebounds!=NULL)ROLLOUT_filebounds->close();
      if(ROLLOUT_showhide!=NULL)ROLLOUT_showhide->close();
      if(ROLLOUT_script!=NULL)ROLLOUT_script->close();
      if(ROLLOUT_config!=NULL)ROLLOUT_config->close();
      if(ROLLOUT_compress!=NULL)ROLLOUT_compress->close();
      if(ROLLOUT_smoke3d!=NULL)ROLLOUT_smoke3d->close();
    }
  }
  else if(menu_id==DIALOG_SCRIPT){
    showscript_dialog=1;
    if(ROLLOUT_script != NULL)ROLLOUT_script->open();
    if(toggle_dialogs==1){
      if(ROLLOUT_time!=NULL)ROLLOUT_time->close();
      if(ROLLOUT_autoload!=NULL)ROLLOUT_autoload->close();
      if(ROLLOUT_filebounds!=NULL)ROLLOUT_filebounds->close();
      if(ROLLOUT_showhide != NULL)ROLLOUT_showhide->close();
      if(ROLLOUT_config!=NULL)ROLLOUT_config->close();
      if(ROLLOUT_compress!=NULL)ROLLOUT_compress->close();
      if(ROLLOUT_smoke3d != NULL)ROLLOUT_smoke3d->close();
    }
  }
  else if(menu_id == DIALOG_SMOKEZIP){
    showzip_dialog=1;
    if(ROLLOUT_compress!=NULL)ROLLOUT_compress->open();
    if(toggle_dialogs==1){
      if(ROLLOUT_time!=NULL)ROLLOUT_time->close();
      if(ROLLOUT_autoload!=NULL)ROLLOUT_autoload->close();
      if(ROLLOUT_filebounds!=NULL)ROLLOUT_filebounds->close();
      if(ROLLOUT_showhide != NULL)ROLLOUT_showhide->close();
      if(ROLLOUT_script != NULL)ROLLOUT_script->close();
      if(ROLLOUT_config!=NULL)ROLLOUT_config->close();
      if(ROLLOUT_smoke3d != NULL)ROLLOUT_smoke3d->close();
    }
  }
  else if(menu_id == DIALOG_3DSMOKE){
    show3dsmoke_dialog=1;
    if(ROLLOUT_filebounds!=NULL)ROLLOUT_filebounds->open();
    if(toggle_dialogs==1){
      if(ROLLOUT_time!=NULL)ROLLOUT_time->close();
      if(ROLLOUT_autoload!=NULL)ROLLOUT_autoload->close();
      if(ROLLOUT_showhide != NULL)ROLLOUT_showhide->close();
      if(ROLLOUT_script != NULL)ROLLOUT_script->close();
      if(ROLLOUT_config!=NULL)ROLLOUT_config->close();
      if(ROLLOUT_compress!=NULL)ROLLOUT_compress->close();
      if(ROLLOUT_smoke3d != NULL)ROLLOUT_smoke3d->open();
    }
  }
  glui_bounds->show();
}

/* ------------------ enable_boundary_glui ------------------------ */

extern "C" void enable_boundary_glui(void){
  ROLLOUT_boundary->enable();
}

/* ------------------ disable_boundary_glui ------------------------ */

extern "C" void disable_boundary_glui(void){
  ROLLOUT_boundary->disable();
}

/* ------------------ update_overwrite ------------------------ */

extern "C" void update_overwrite(void){
  if(CHECKBOX_overwrite_all!=NULL)CHECKBOX_overwrite_all->set_int_val(overwrite_all);
  if(CHECKBOX_compress_autoloaded!=NULL)CHECKBOX_compress_autoloaded->set_int_val(compress_autoloaded);
}

/* ------------------ hide_glui_bounds ------------------------ */

extern "C" void hide_glui_bounds(void){
  if(glui_bounds!=NULL)glui_bounds->hide();
  showbounds_dialog=0;
  showscript_dialog=0;
  showzip_dialog=0;
}

/* ------------------ update_vector_widgets ------------------------ */

extern "C" void update_vector_widgets(void){
  PLOT3D_CB(UPDATE_VECTOR_FROM_SMV);
  Slice_CB(UPDATE_VECTOR_FROM_SMV);
}

/* ------------------ update_plot3d_display ------------------------ */

extern "C" void update_plot3d_display(void){
  if(RADIO_plot3d_display!=NULL)RADIO_plot3d_display->set_int_val(contour_type);
}

/* ------------------ open_volsmokeplane ------------------------ */

extern "C" void open_volsmokepanel(void){
  if(ROLLOUT_volsmoke3d!=NULL){
    ROLLOUT_volsmoke3d->open();
  }
}


/* ------------------ close_volsmokeplane ------------------------ */

extern "C" void close_volsmokepanel(void){
  if(ROLLOUT_volsmoke3d!=NULL){
    ROLLOUT_volsmoke3d->close();
  }
}

/* ------------------ open_smokepanel ------------------------ */

extern "C" void open_smokepanel(void){
  if(ROLLOUT_smoke3d!=NULL){
    ROLLOUT_smoke3d->open();
  }
}

/* ------------------ close_smokepanel ------------------------ */

extern "C" void close_smokepanel(void){
  if(ROLLOUT_smoke3d!=NULL){
    ROLLOUT_smoke3d->close();
  }
}

/* ------------------ open_smokezippanel ------------------------ */

extern "C" void open_smokezippanel(void){
  if(ROLLOUT_compress!=NULL){
    ROLLOUT_compress->open();
  }
}


/* ------------------ close_smokezippanel ------------------------ */

extern "C" void close_smokezippanel(void){
  if(ROLLOUT_compress!=NULL){
    ROLLOUT_compress->close();
  }
}
/* ------------------ updateGluiTimeBounds ------------------------ */

extern "C" void updateGluiTimeBounds(float time_min, float time_max){
  if(SPINNER_timebounds!=NULL){
    SPINNER_timebounds->set_float_limits(time_min,time_max);
  }
}

/* ------------------ update_tbounds ------------------------ */

extern "C" void update_tbounds(void){
  settmin_p=use_tload_begin;
  settmax_p=use_tload_end;
  tmin_p=tload_begin;
  tmax_p=tload_end;

  settmin_s=use_tload_begin;
  settmax_s=use_tload_end;
  tmin_s=tload_begin;
  tmax_s=tload_end;

  settmin_i=use_tload_begin;
  settmax_i=use_tload_end;
  tmin_i=tload_begin;
  tmax_i=tload_end;

  settmin_s=use_tload_begin;
  settmax_s=use_tload_end;
  tmin_s=tload_begin;
  tmax_s=tload_end;

  settmin_b=use_tload_begin;
  settmax_b=use_tload_end;
  tmin_b=tload_begin;
  tmax_b=tload_end;

  if(use_tload_skip==1){
    smoke3dframeskip=tload_skip;
    boundframeskip=tload_skip;
    isoframeskip_global=tload_skip;
    partframeskip=tload_skip;
    evacframeskip=tload_skip;
    sliceframeskip=tload_skip;
  }
  else{
    smoke3dframeskip=0;
    boundframeskip=0;
    isoframeskip_global=0;
    partframeskip=0;
    evacframeskip=0;
    sliceframeskip=0;
  }

  Smoke3D_CB(FRAMELOADING);
  Bound_CB(FRAMELOADING);
  Iso_CB(FRAMELOADING);
  PART_CB(FRAMELOADING);
  Slice_CB(FRAMELOADING);
}


/* ------------------ FileShow_CB ------------------------ */

extern "C" void FileShow_CB(int var){
  updatemenu = 1;
  switch(var){
  case  FILESHOW_plot3d:
    switch(showhide_option){
    case 0:
    case 1:
      Plot3DShowMenu(SHOWALL_PLOT3D);
      break;
    case 2:
      Plot3DShowMenu(HIDEALL_PLOT3D);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case FILESHOW_evac:
    switch(showhide_option){
    case 0:
      EvacShowMenu(SHOWALL_EVAC);
      break;
    case 1:
      EvacShowMenu(SHOWALL_EVAC);
      if(npartloaded != 0)ParticleShowMenu(HIDEALL_PARTICLE);
      if(nsmoke3dloaded != 0)Smoke3DShowMenu(HIDEALL_SMOKE3D);
      if(nisoloaded != 0)IsoShowMenu(HIDEALL_ISO);
      if(nsliceloaded != 0)ShowHideSliceMenu(HIDEALL_SLICE);
      if(nvsliceloaded != 0)ShowVSliceMenu(HIDEALL_VSLICE);
      if(npatchloaded != 0)ShowPatchMenu(HIDEALL_BOUNDARY);
      break;
    case 2:
      EvacShowMenu(HIDEALL_EVAC);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case  FILESHOW_particle:
    switch(showhide_option){
    case 0:
      ParticleShowMenu(SHOWALL_PARTICLE);
      break;
    case 1:
      ParticleShowMenu(SHOWALL_PARTICLE);
      if(nevacloaded != 0)EvacShowMenu(HIDEALL_EVAC);
      if(nsmoke3dloaded != 0)Smoke3DShowMenu(HIDEALL_SMOKE3D);
      if(nisoloaded != 0)IsoShowMenu(HIDEALL_ISO);
      if(nsliceloaded != 0)ShowHideSliceMenu(HIDEALL_SLICE);
      if(nvsliceloaded != 0)ShowVSliceMenu(HIDEALL_VSLICE);
      if(npatchloaded != 0)ShowPatchMenu(HIDEALL_BOUNDARY);
      break;
    case 2:
      ParticleShowMenu(HIDEALL_PARTICLE);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case  FILESHOW_slice:
    switch(showhide_option){
    case 0:
      ShowHideSliceMenu(SHOWALL_SLICE);
      break;
    case 1:
      ShowHideSliceMenu(SHOWALL_SLICE);
      if(nevacloaded != 0)EvacShowMenu(HIDEALL_EVAC);
      if(nvsliceloaded != 0)ShowVSliceMenu(HIDEALL_VSLICE);
      if(npatchloaded != 0)ShowPatchMenu(HIDEALL_BOUNDARY);
      if(nsmoke3dloaded != 0)Smoke3DShowMenu(HIDEALL_SMOKE3D);
      if(nisoloaded != 0)IsoShowMenu(HIDEALL_ISO);
      if(npartloaded != 0)ParticleShowMenu(HIDEALL_PARTICLE);
      break;
    case 2:
      ShowHideSliceMenu(HIDEALL_SLICE);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case  FILESHOW_vslice:
    switch(showhide_option){
    case 0:
      ShowVSliceMenu(SHOWALL_VSLICE);
      break;
    case 1:
      ShowVSliceMenu(SHOWALL_VSLICE);
      if(nevacloaded != 0)EvacShowMenu(HIDEALL_EVAC);
      if(npatchloaded != 0)ShowPatchMenu(HIDEALL_BOUNDARY);
      if(nsmoke3dloaded != 0)Smoke3DShowMenu(HIDEALL_SMOKE3D);
      if(nisoloaded != 0)IsoShowMenu(HIDEALL_ISO);
      if(npartloaded != 0)ParticleShowMenu(HIDEALL_PARTICLE);
      if(nsliceloaded != 0)ShowHideSliceMenu(HIDEALL_SLICE);
      break;
    case 2:
      ShowHideSliceMenu(HIDEALL_SLICE);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case  FILESHOW_boundary:
    switch(showhide_option){
    case 0:
      ShowPatchMenu(SHOWALL_BOUNDARY);
      break;
    case 1:
      ShowPatchMenu(SHOWALL_BOUNDARY);
      if(nevacloaded != 0)EvacShowMenu(HIDEALL_EVAC);
      if(nsmoke3dloaded != 0)Smoke3DShowMenu(HIDEALL_SMOKE3D);
      if(npartloaded != 0)ParticleShowMenu(HIDEALL_PARTICLE);
      if(nvsliceloaded != 0)ShowVSliceMenu(HIDEALL_VSLICE);
      if(nsliceloaded != 0)ShowHideSliceMenu(HIDEALL_SLICE);
      if(nisoloaded != 0)IsoShowMenu(HIDEALL_ISO);
      break;
    case 2:
      ShowPatchMenu(HIDEALL_BOUNDARY);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case  FILESHOW_3dsmoke:
    switch(showhide_option){
    case 0:
      Smoke3DShowMenu(SHOWALL_SMOKE3D);
      break;
    case 1:
      Smoke3DShowMenu(SHOWALL_SMOKE3D);
      if(nevacloaded != 0)EvacShowMenu(HIDEALL_EVAC);
      if(npatchloaded != 0)ShowPatchMenu(HIDEALL_BOUNDARY);
      if(npartloaded != 0)ParticleShowMenu(HIDEALL_PARTICLE);
      if(nvsliceloaded != 0)ShowVSliceMenu(HIDEALL_VSLICE);
      if(nsliceloaded != 0)ShowHideSliceMenu(HIDEALL_SLICE);
      if(nisoloaded != 0)IsoShowMenu(HIDEALL_ISO);
      break;
    case 2:
      Smoke3DShowMenu(HIDEALL_SMOKE3D);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  case  FILESHOW_isosurface:
    switch(showhide_option){
    case 0:
      IsoShowMenu(SHOWALL_ISO);
      break;
    case 1:
      IsoShowMenu(SHOWALL_ISO);
      if(nevacloaded != 0)EvacShowMenu(HIDEALL_EVAC);
      if(nsmoke3dloaded != 0)Smoke3DShowMenu(HIDEALL_SMOKE3D);
      if(npatchloaded != 0)ShowPatchMenu(HIDEALL_BOUNDARY);
      if(npartloaded != 0)ParticleShowMenu(HIDEALL_PARTICLE);
      if(nvsliceloaded != 0)ShowVSliceMenu(HIDEALL_VSLICE);
      if(nsliceloaded != 0)ShowHideSliceMenu(HIDEALL_SLICE);
      break;
    case 2:
      IsoShowMenu(HIDEALL_ISO);
      break;
    default:
      ASSERT(FFALSE);
      break;
    }
    break;
  default:
    break;
  }
}

/* ------------------ glui_labels_rename ------------------------ */

extern "C" void glui_bounds_rename(void){

  if((npartinfo > 0) || nsliceinfo > 0 || nvsliceinfo > 0 || nisoinfo > 0 || npatchinfo || nsmoke3dinfo > 0 || nplot3dinfo > 0){
    ROLLOUT_showhide->set_name(_("Show/Hide Loaded Files"));
  }

  //    glui_labels->add_radiobutton_to_group(RADIO_showhide,_("Show"));
  //    glui_labels->add_radiobutton_to_group(RADIO_showhide,_("Show Only"));
  //    glui_labels->add_radiobutton_to_group(RADIO_showhide,_("Hide"));

}

/* ------------------ update_fileload  ------------------------ */

extern "C" void update_fileload(void){
  int i;
  partdata *parti;
  slicedata *slicei;
  isodata *isoi;
  patchdata *patchi;
  smoke3ddata *smoke3di;
  plot3ddata *plot3di;
  vslicedata *vslicei;

  npartloaded = 0;
  nevacloaded = 0;
  for(i = 0; i < npartinfo; i++){
    parti = partinfo + i;
    if(parti->loaded == 1 && parti->evac == 0){
      npartloaded++;
    }
    if(parti->loaded == 1 && parti->evac == 1){
      nevacloaded++;
    }
  }

  nsliceloaded = 0;
  for(i = 0; i < nsliceinfo; i++){
    slicei = sliceinfo + i;
    if(slicei->loaded == 1){
      nsliceloaded++;
    }
  }

  nvsliceloaded = 0;
  for(i = 0; i < nvsliceinfo; i++){
    vslicei = vsliceinfo + i;
    if(vslicei->loaded == 1){
      nvsliceloaded++;
    }
  }

  nisoloaded = 0;
  for(i = 0; i < nisoinfo; i++){
    isoi = isoinfo + i;
    if(isoi->loaded == 1){
      nisoloaded++;
    }
  }

  npatchloaded = 0;
  for(i = 0; i < npatchinfo; i++){
    patchi = patchinfo + i;
    if(patchi->loaded == 1){
      npatchloaded++;
    }
  }

  nsmoke3dloaded = 0;
  for(i = 0; i < nsmoke3dinfo; i++){
    smoke3di = smoke3dinfo + i;
    if(smoke3di->loaded == 1){
      nsmoke3dloaded++;
    }
  }

  nplot3dloaded = 0;
  for(i = 0; i < nplot3dinfo; i++){
    plot3di = plot3dinfo + i;
    if(plot3di->loaded == 1){
      nplot3dloaded++;
    }
  }
}


/* ------------------ update_showhidebuttons ------------------------ */

extern "C" void update_showhidebuttons(void){

  update_fileload();
//  if(CHECKBOX_label_3 != NULL){
//    CHECKBOX_label_3->set_int_val(hide_overlaps);
//  }
  if(BUTTON_PART != NULL){
    if(npartloaded == 0){
      BUTTON_PART->disable();
    }
    else{
      BUTTON_PART->enable();
    }
  }

  if(BUTTON_SLICE != NULL){
    if(nsliceloaded == 0){
      BUTTON_SLICE->disable();
    }
    else{
      BUTTON_SLICE->enable();
    }
  }

  if(BUTTON_VSLICE != NULL){
    if(nvsliceloaded == 0){
      BUTTON_VSLICE->disable();
    }
    else{
      BUTTON_VSLICE->enable();
    }
  }

  if(BUTTON_ISO != NULL){
    if(nisoloaded == 0){
      BUTTON_ISO->disable();
    }
    else{
      BUTTON_ISO->enable();
    }
  }

  if(BUTTON_BOUNDARY != NULL){
    if(npatchloaded == 0){
      BUTTON_BOUNDARY->disable();
    }
    else{
      BUTTON_BOUNDARY->enable();
    }
  }

  if(BUTTON_3DSMOKE != NULL){
    if(nsmoke3dloaded == 0){
      BUTTON_3DSMOKE->disable();
    }
    else{
      BUTTON_3DSMOKE->enable();
    }
  }

  if(BUTTON_PLOT3D != NULL){
    if(nplot3dloaded == 0){
      BUTTON_PLOT3D->disable();
    }
    else{
      BUTTON_PLOT3D->enable();
    }
  }

  if(nplot3dloaded == 0 && nsmoke3dloaded == 0 && nisoloaded == 0 && nsliceloaded == 0 && npartloaded == 0 && npatchloaded == 0){
    if(RADIO_showhide != NULL)RADIO_showhide->disable();
  }
  else{
    if(RADIO_showhide != NULL)RADIO_showhide->enable();
  }
}



