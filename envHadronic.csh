# === FILE TO BE REMOVED IN G4 11.0 ===
#
# for neutronHP and particleHP
#
unsetenv G4NEUTRONHP_SKIP_MISSING_ISOTOPES 
unsetenv G4NEUTRONHP_DO_NOT_ADJUST_FINAL_STATE
unsetenv G4NEUTRONHP_USE_ONLY_PHOTONEVAPORATION
unsetenv G4NEUTRONHP_NELECT_DOPPLER
unsetenv G4NEUTRONHP_PRODUCE_FISSION_FRAGMENTS

unsetenv G4PHP_DO_NOT_ADJUST_FINAL_STATE
unsetenv G4PHP_NELECT_DOPPLER

#
# for Bertini cascade
#
unsetenv G4CASCADE_USE_PRECOMPOUND

env |grep G4NEUTRONHP
env |grep G4PHP
env |grep G4CASCADE

# Add Geant4 source to PATH for IDE builds
setenv PATH "/home/rsultano/soft/geant4/geant4-v11.2.1/source/:$PATH"
