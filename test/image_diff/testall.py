import sys
import os
import maya.standalone
import maya.OpenMaya as OpenMaya
import maya.cmds as cmds
import testone_by_mayapy as MyTestOne
import liqTestLog as mLiqlog


def configMayaEnv():
    #liquid maya
    os.environ["LIQUID_ROOT"]    = "E:/dev/autodesk/maya/myplugin/project/liquid_"
    os.environ["LIQUID_MEL"]     = os.environ["LIQUID_ROOT"]+"/2.3.3/mel"
    os.environ["LIQUID_RM_MEL"]  = os.environ["LIQUID_ROOT"]+"/2.3.3/src/renderman/mel"
    os.environ["LIQUID_ER_MEL"]  = os.environ["LIQUID_ROOT"]+"/2.3.3/src/elvishray/mel"
    os.environ["LIQUID_LUX_MEL"] = os.environ["LIQUID_ROOT"]+"/2.3.3/src/luxrenderer/mel"
    os.environ["LIQUID_GEN"]     = os.environ["LIQUID_ROOT"]+"/2.3.3/lib/plugins"
    os.environ["LIQUID_MLL"]     = os.environ["LIQUID_ROOT"]+"/2.3.3/bin/prman/win32"
    os.environ["LIQUID_ICON"]    = os.environ["LIQUID_ROOT"]+"/2.3.3/icons"
    os.environ["LIQUIDHOME"]     = os.environ["LIQUID_ROOT"]+"/2.3.3"
    os.environ["LIQUID_GLOBALS_OVERRIDE"] = os.environ["LIQUID_ROOT"]+"/2.3.3/renderers"
    #elvishray
    os.environ["ELVISHRAY_BIN"] = os.environ["LIQUID_ROOT"]+"/dependence/elvishray/bin"
    
    # PATH
    os.environ["PATH"] = os.environ["ELVISHRAY_BIN"]+";"+os.environ["PATH"]
    # MAYA_SCRIPT_PATH
    if os.environ.get('MAYA_SCRIPT_PATH') == None:
        os.environ["MAYA_SCRIPT_PATH"] =os.environ["LIQUID_RM_MEL"]+";"+os.environ["LIQUID_ER_MEL"]+";"+os.environ["LIQUID_LUX_MEL"]+";"+os.environ["LIQUID_GEN"]+";"+os.environ["LIQUID_MEL"];
    else:
        os.environ["MAYA_SCRIPT_PATH"] =os.environ["LIQUID_RM_MEL"]+";"+os.environ["LIQUID_ER_MEL"]+";"+os.environ["LIQUID_LUX_MEL"]+";"+os.environ["LIQUID_GEN"]+";"+os.environ["LIQUID_MEL"]+";"+os.environ["MAYA_SCRIPT_PATH"];
    # MAYA_PLUG_IN_PATH
    if os.environ.get('MAYA_PLUG_IN_PATH') == None:
        os.environ["MAYA_PLUG_IN_PATH"]=os.environ["LIQUID_MLL"]
    else:
        os.environ["MAYA_PLUG_IN_PATH"]=os.environ["LIQUID_MLL"]+";"+os.environ["MAYA_PLUG_IN_PATH"]
    # XBMLANGPATH
    if os.environ.get('MAYA_PLUG_IN_PATH') == None:
        os.environ["XBMLANGPATH"]=os.environ["LIQUID_ICON"]
    else:
        os.environ["XBMLANGPATH"]=os.environ["LIQUID_ICON"]+";"+os.environ["XBMLANGPATH"]
    #
    os.environ["MAYA_LOCATION"] = "D:/Program Files/Autodesk/Maya2009"
    os.environ["MAYA_PATH2009"] = "D:/Program Files/Autodesk/Maya2009"
    os.environ["PYTHONHOME"]    = "D:/Program Files/Autodesk/Maya2009/Python"
    os.environ["PYTHONPATH"]    = "D:/Program Files/Autodesk/Maya2009/bin/python25.zip;D:/Program Files/Autodesk/Maya2009/Python/lib/site-packages;"



def initialize():
    try:
        maya.standalone.initialize( name='python' )
    except:
        sys.stderr.write( "Failed in initialize standalone application" )
        raise



def test_all_scene():
    LIQUID_ROOT="E:/dev/Autodesk/maya/myplugin/project/liquid_"

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_motionblur/transform/deform.ma"
    #MyTestOne.test_one_scene(testFile, "")
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_motionblur/transform/transform.ma"
    #MyTestOne.test_one_scene(testFile, "")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_rm_texture/check10_tex.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_animation/checker_anim.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_pl/test_shader_pl.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_pl/test_shader_pl_connection.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_pl/test_type_array.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")

    #[Warning] test_displace_pl.ma lead mayabatch crash!
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_pl/test_displace_pl.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_graph/er_rm_shader_graph.ma"
    #MyTestOne.test_one_scene(testFile, "")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_shader/three_boxes.ma"
    #MyTestOne.test_one_scene(testFile, "")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_shader/lambert_transparent.ma"
    #MyTestOne.test_one_scene(testFile, "")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_pointlight.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_spotlight.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_dof/er_dof2.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_env/er_env_simple_env.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_env/er_env_physicalsky.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_rm_displacement/test_displacement.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_rm_light/rm_spotlight.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")

    #testFile=LIQUID_ROOT+"/2.3.3/test/hair/test_shaveHair(shave_v61).ma"
    #MyTestOne.test_one_scene(testFile, "renderman")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_pfxhair/test_pfxHair.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")

    #animation
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_animation/checker_anim.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_animation/er_anim.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_simple.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    MyTestOne.test_one_scene(testFile, "renderman")

def main():
    configMayaEnv()
    mLiqlog.flog("configMayaEnv() done.")
    initialize()
    mLiqlog.flog("initialize() done.")

    mLiqlog.flog("test_all_scene() begin.")
    test_all_scene()
    mLiqlog.flog("test_all_scene() end.")
    #raw_input("test_all_cases() done. Press ENTER to exit")


if __name__ == '__main__':
    main()
    #test_getStanderImage()
#_render_bat(testFile, "renderman")
#