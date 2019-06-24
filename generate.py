import os
import shutil
import subprocess
from pathlib import Path
TARGET_FOLDER = "../PQClean/crypto_sign/"

ALL = ['_USE_GF16', '_RAINBOW16_32_32_32', '_RAINBOW256_68_36_36', '_RAINBOW_CLASSIC',
       '_RAINBOW_CYCLIC', '_RAINBOW_CYCLIC_COMPRESSED']
params = [
    {'name': 'rainbowIa-classic', 'def' : ['_RAINBOW_CLASSIC', '_USE_GF16', '_RAINBOW16_32_32_32', '_HASH_LEN=32']},
    {'name': 'rainbowIa-cyclic', 'def' :['_RAINBOW_CYCLIC','_USE_GF16', '_RAINBOW16_32_32_32','_HASH_LEN=32']},
    {'name': 'rainbowIa-cyclic-compressed', 'def' :['_RAINBOW_CYCLIC_COMPRESSED','_USE_GF16', '_RAINBOW16_32_32_32','_HASH_LEN=32']},
    {'name': 'rainbowIIIc-classic', 'def' : ['_RAINBOW_CLASSIC','_RAINBOW256_68_36_36', '_HASH_LEN=48']},
    {'name': 'rainbowIIIc-cyclic', 'def' : ['_RAINBOW_CYCLIC','_RAINBOW256_68_36_36', '_HASH_LEN=48']},
    {'name': 'rainbowIIIc-cyclic-compressed', 'def' : ['_RAINBOW_CYCLIC_COMPRESSED','_RAINBOW256_68_36_36', '_HASH_LEN=48']},
    {'name': 'rainbowVc-classic', 'def' : ['_RAINBOW_CLASSIC','_RAINBOW256_92_48_48', '_HASH_LEN=64']},
    {'name': 'rainbowVc-cyclic', 'def' : ['_RAINBOW_CYCLIC','_RAINBOW256_92_48_48', '_HASH_LEN=64']},
    {'name': 'rainbowVc-cyclic-compressed', 'def' : ['_RAINBOW_CYCLIC_COMPRESSED','_RAINBOW256_92_48_48', '_HASH_LEN=64']},
]
for param in params:
    parameterSet = param['name']
    pqcleanDir = f"{TARGET_FOLDER}/{parameterSet}/clean/"

    # delete old files
    if Path(pqcleanDir).exists():
        shutil.rmtree(pqcleanDir)
    os.makedirs(pqcleanDir)

    nmspc = "PQCLEAN_"+parameterSet.upper().replace("-", "")+"_CLEAN"
    for f in os.listdir(f"rainbow"):
        # copy over common source files
        shutil.copyfile(f"rainbow/{f}", f"{pqcleanDir}/{f}")

        # namespace source files
        cmd = f"sed -i 's/PQCLEAN_NAMESPACE/{nmspc}/g' {pqcleanDir}/{f}"
        subprocess.call(cmd, shell=True)

        # remove preprocessor conditionals
        undefs = [x for x in ALL if x not in param['def']]
        cmd = f"unifdef -m " + " ".join(["-D"+d for d in param['def']]) + " " + " ".join(["-U"+d for d in undefs]) +  f" {pqcleanDir}/{f}"
        print(cmd)
        subprocess.call(cmd, shell=True)

    # copy over param specific files
    for f in os.listdir(f"params/{parameterSet}"):
        shutil.copyfile(f"params/{parameterSet}/{f}", f"{pqcleanDir}/{f}")


    # copy over Makefiles
    for f in os.listdir(f"make"):
        shutil.copyfile(f"make/{f}", f"{pqcleanDir}/{f}")

        # replace lib name
        cmd = f"sed -i 's/SCHEME_NAME/{parameterSet}/g' {pqcleanDir}/{f}"
        subprocess.call(cmd, shell=True)

    # run astyle to fix formatting due to namespace
    cmd = f"astyle --project {pqcleanDir}/*.[ch]"
    subprocess.call(cmd, shell=True)
