-- Configuration file for the AT91SAM7S256 backend

local cpumode = ( builder:get_option( 'cpumode' ) or 'thumb' ):lower()

specific_files = "board_cstartup.s board_lowlevel.c board_memories.c usart.c pmc.c pio.c platform.c tc.c pwmc.c aic.c dbgu.c platform_int.c"
local ldscript
if comp.cpu:upper() == 'AT91SAM7S256' then
  ldscript = "at91sam7s256/flash_sam7x.lds"
  addm( 'at91sam7s256' )
else
  print( sf( "Invalid AT91SAM7S CPU %s", comp.cpu ) )
  os.exit( -1 )
end

addm( { 'NOASSERT','NOTRACE' } )
  
-- Prepend with path
specific_files = utils.prepend_path( specific_files, sf( "src/platform/%s", platform ) )
specific_files = specific_files .. " src/platform/arm_utils.s src/platform/arm_cortex_interrupts.c"
ldscript = sf( "src/platform/%s/%s", platform, ldscript )

addcf( { '-ffunction-sections', '-fdata-sections', '-fno-strict-aliasing', '-Wall' } )
addlf( { '-nostartfiles', '-nostdlib', '-T', ldscript, '-Wl,--gc-sections', '-Wl,--allow-multiple-definition' } )
addaf( { '-x', 'assembler-with-cpp', '-Wall' } )
addlib( { 'c','gcc','m' } )

local target_flags = '-mcpu=arm7tdmi'
if cpumode == 'thumb' then
  target_flags = target_flags .. ' -mthumb'
  addm( 'CPUMODE_THUMB' )
else
  addm( 'CPUMODE_ARM' )
end

-- Configure general flags for target
addcf( target_flags )
addlf( { target_flags, '-Wl,-e,entry' } )
addaf( { target_flags, '-D__ASSEMBLY__' } )

-- Toolset data
tools.at91sam7s = {}

-- Array of file names that will be checked against the 'prog' target; their absence will force a rebuild
tools.at91sam7s.prog_flist = { output .. ".bin" }

-- We use 'gcc' as the assembler
toolset.asm = toolset.compile

