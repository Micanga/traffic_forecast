library("forecast")
source('utils.R')
# HARIMA FORECASTING PACKAGE ..................
predictHISTO <- function(flow,day,ntrainning_days,npredictions){
	sum = 0
	i = day - END_PERIOD*ntrainning_days
	
	#trainning
	if(i > 0){
		while(i < day){
			sum = sum + flow[i,1]
			i = i + END_PERIOD*N_WEEK_DAYS
		}
	}
	else{
		return(-1)
	}

	#predicting
	for(i in 1:npredictions){
		sum = sum + sum/(ntrainning_days+i-1)
	}
	predict = sum/(ntrainning_days+npredictions-1)
	return(predict)
}

runtestHARIMA <- function(npredictions,filename){
	#1. Starting the test
	print(paste("Start test - file :",filename))
	if(file.exists(filename)){
		#2. Opening the necessary file
		file <- read.csv(filename,header=F)

		#3. Defining the start dataset
		flow <- c()
		for( i in 1:(END_PERIOD*3))
			flow <- c(flow,file[i,1])

		#4. Starting the prediction
		Error = 0
		HISTOError = 0
		ARIMAError = 0
		TotalFlow = 0
		for(i in ((END_PERIOD*3)):((END_PERIOD*4)-npredictions)) {
			if(length(flow) >= 3){
				forecastARIMA <- predictARIMA(flow,npredictions)
				forecastHISTO = predictHISTO(file,i,3,npredictions)

				if(is.finite(forecastARIMA$pred[1]) && is.finite(forecastHISTO) && forecastHISTO != -1){
					if(HISTOError > ARIMAError){
						Error = Error + abs(forecastARIMA$pred[1]-file[i+npredictions,1])
					}
					else{
						Error = Error + abs(forecastHISTO-file[i+npredictions,1])
					}

					ARIMAError <- abs(file[i+npredictions,1] - as.numeric(forecastARIMA$pred[1]))
					HISTOError <- abs(file[i+npredictions,1] - forecastHISTO) 
		    	}
				else{
					ARIMAError = 0
					HISTOError = 0
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