AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I auxdir
SUBDIRS    = dep src extra imagegw etc_files

if WITH_SLURM
    SUBDIRS += wlm_integration/slurm
endif WITH_SLURM

sysconf_DATA=udiRoot.conf.example
