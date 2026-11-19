[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$VcpkgExe
)

$ErrorActionPreference = 'Stop'
$vcpkgPath = (Resolve-Path -LiteralPath $VcpkgExe).Path
$repositoryRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
$installRoot = Join-Path $repositoryRoot 'vcpkg_installed/ffmpeg'

# Keep the experimental FFmpeg dependency separate from the main manifest.
& $vcpkgPath install '--triplet=x86-ffmpeg' `
    "--overlay-triplets=$(Join-Path $PSScriptRoot 'triplets')" `
    "--x-manifest-root=$PSScriptRoot" "--x-install-root=$installRoot"
if ($LASTEXITCODE -ne 0) {
    throw "FFmpeg dependency installation failed (exit code $LASTEXITCODE)."
}
Write-Host "FFmpeg libraries are ready at $(Join-Path $installRoot 'x86-ffmpeg')."
