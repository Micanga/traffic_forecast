library("forecast")
source('utils.R')
# ARIMA FORECASTING PACKAGE ..................
predictARIMA <- function(flow,ndays){
	return(predict(arima(flow,order = c(0,1,2)),n.ahead = ndays))
}

runtestARIMA <- function(npredictions,filename){
	#1. Starting the ARIMA test
	print(paste("Start test - file :",filename))
	if(file.exists(filename)){
		#a. Opening the data file
		file <- read.csv(filename,header=F)
		
		#b. defining the start dataset
		flow <- c()
		for( i in 1:(END_PERIOD*3))
			flow <- c(flow,file[i,1])

		#c. starting the forecasting
		Error = 0
		TotalFlow = 0
		for(i in ((END_PERIOD*3)):((END_PERIOD*4)-3)) {
			if(length(flow) >= 3){
				if(!is.nan(file[i+npredictions,1])){
					forecast <- predict(arima(flow,order = c(0,1,2)),n.ahead = npredictions)
					Error = Error + abs(forecast$pred[1]-file[i+npredictions,1])
				}
			}
			if(!is.nan(file[i+npredictions,1]) && is.numeric(file[i+1,1])){
				TotalFlow = TotalFlow + file[i+npredictions,1]
				flow <- c(flow,file[i+npredictions,1])
			}
		}
		#print(100*Error/TotalFlow)
		#write(c(100*Error/TotalFlow),file=outputfilename("ARIMA",npredictions),ncolumns = 1,append = TRUE,sep = "")
	}
	else{
		print("> File doesn't exist.")
	}
}