# PowerShell 스크립트: AddHeadersToFilter.ps1

param (
    [string]$projectDir,
    [string]$filterName = "Headers"
)

# .vcxproj.filters 파일 경로
$vcxprojFiltersPath = Join-Path -Path $projectDir -ChildPath "*.vcxproj.filters"

# .h 파일 목록 가져오기
$headerFiles = Get-ChildItem -Path $projectDir -Recurse -Filter "*.h"

# .vcxproj.filters 파일 열기
$vcxprojFiltersFile = Get-Content -Path $vcxprojFiltersPath

# ItemGroup 태그 찾기
$itemGroupTag = "<ItemGroup>"
$endItemGroupTag = "</ItemGroup>"

# 새로운 필터 항목 추가
foreach ($file in $headerFiles) {
    $relativePath = $file.FullName.Substring($projectDir.Length + 1)
    $newFilterEntry = "  <ClInclude Include=`"$relativePath`">
    <Filter>$filterName</Filter>
  </ClInclude>"
    $vcxprojFiltersFile = $vcxprojFiltersFile -replace "($itemGroupTag)", "`$1`n$newFilterEntry"
}

# .vcxproj.filters 파일 저장
Set-Content -Path $vcxprojFiltersPath -Value $vcxprojFiltersFile