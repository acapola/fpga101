#find out the directory of this script
#that allow us to find the input files no matter what is the current directory
set scriptDir [file dirname [info script]]
puts $::tgppScriptEnv::dir

#create a tgppInterp, we will use it to run all TGPP file examples
tgppInterp create pp

#all files are in utf-8 encoding (without BOM)
pp setOutEncoding utf-8

#process each tgpp file and copy the resulting files in both kit and device projects
set files [list sfr_read_cmd.tgpp.h]

#get the file list 
set files [lsort [glob -directory $scriptDir -type f *.tgpp.*]]

#set the output directories
set outDir  [file join $::tgppScriptEnv::dir .. device generated]
set outDir2 [file join $::tgppScriptEnv::dir .. kit    generated]

#clear them if they already exist
file delete -force -- $outDir
file delete -force -- $outDir2

#create output directories
file mkdir $outDir
file mkdir $outDir2

#set outDir as our primary output directory
pp setBaseDir $outDir

foreach file $files {
	array set r [pp file $file ""]
	foreach f [array names r] {
		puts $f
		file attributes $f -readonly 1
		set dest [file join $outDir2 [file tail $f]]
		if {[catch {file copy -force $f $dest} sError]} {
			error "file copy failed: err:$sError to:$dest"
		}
		file attributes $dest -readonly 1
	}
}

pp delete
