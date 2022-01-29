# do not look for pogfiles in this directory
# exclude_paths = ["dont_include"]  

# these files/directories shall be imported, even if they're out of path or if they're 
# include_paths = ["test_simple.py", "dont_include/test_anyway/"] 

# autoindex = False # change this to true to automatically search for pogfile.py in inferior directories.

unreal_engine_path = "D:/EpicGames/UE_4.27/"
unreal_engine_batchfiles = unreal_engine_path + "Engine/Build/BatchFiles/"
build_bat = unreal_engine_batchfiles + "Build.bat"
editor_exe = unreal_engine_path + "Engine/Binaries/Win64/UE4Editor.exe"
this_uproj = orig_dir + "/Emb.uproject"

if os.path.exists(orig_dir + '/pog_config.json'):
    import json
    
    with open(orig_dir + "/pog_config.json") as f:
        x = json.load(f)
        if "unreal_engine_path" in x:
            unreal_engine_path = x['unreal_engine_path']

def uedtior_do(*args):
    env.run(editor_exe, this_uproj, *args)

@job(desc="Runs the editor build of unreal engine")
def build_editor():
    env.run(build_bat, "EmbEditor", "Win64", "Development", this_uproj, '-waitmutex', '-NoHotReload')


@job('build_editor', default=False, desc="open the unreal engine editor for this project")
def editor():
    env.run(editor_exe, this_uproj, '-log')


@job('build_editor', default=False, desc="open the unreal engine editor for this project")
def editor_game():
    env.run(editor_exe, this_uproj, '-log', '-game', '-windowed', 'resx=1280', 'resy=720')


@job(desc="This performs the complete game build", default=False)
def build_and_cook_game():
    env.run(build_bat, "Emb", "Win64", "Development", this_uproj, '-waitmutex', '-NoHotReload')
    env.run(editor_exe, this_uproj, '-run=cook', '-targetplatform=WindowsNoEditor')


@job(desc="Build and run the game in a standalone environment", default=False)
def run_standalone():
    env.run('Binaries/Win64/AbyssalSanityService.exe', '-windowed', 'resx=1280', 'resy=720')

@job(desc="Does a mass resave", default=False)
def try_to_fix_blueprints():
    env.run(editor_exe, this_uproj, '-run=ResavePackages', '-fixupredirects', '-autocheckout', '-projectonly', 'unattended')

@job('build_editor', 'build_and_cook_game', default=False)
def build_all():
    pass

@job('build_editor', default=False)
def run_tests():
    uedtior_do('-editortest', '-execcmds="Automation RunTests"');
    pass

@job(default=False)
def upload_to_itch():
    env.run("butler", 'push', 'Builds/WindowsNoEditor', 'peterino2/CardGame:windows')
    pass