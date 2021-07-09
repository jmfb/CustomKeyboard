$ErrorActionPreference = "Stop"

try {
	$outputFile = "Tests.exe"

	Write-Host "$(Get-Date)] Gathering source files..."
	$sourceFiles = Get-ChildItem *.cpp | ForEach { $_.Name }
	Write-Host $sourceFiles

	Write-Host "[$(Get-Date)] Compiling tests..."
	& g++ -std=c++2a -o $outputFile $sourceFiles
	if ($lastexitcode -ne 0) {
		exit -1
	}

	Write-Host "[$(Get-Date)] Running tests..."
	& ".\$outputFile"
	if ($lastexitcode -ne 0) {
		exit -1
	}

	Write-Host "[$(Get-Date)] All tests passed successfully."
	exit 0
} catch {
	Write-Host "[$(Get-Date)] Exception" -ForegroundColor Red
	Write-Host $_ -ForegroundColor Red
	exit -1
}
