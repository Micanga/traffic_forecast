library("e1071")
source('utils.R')
# SVM FORECASTING PACKAGE ..................
runtestSVM <- function(filename){
	#1. Starting the test
	if(file.exists(filename)){
		#2. Opening the necessary file
		file <- read.csv(filename,header=F)

		#3. Defining the start dataset
		y <- c()
		x <- c()
		for( i in 1:(96*7*3)){
			if(i <= (96*7)){
				y <- c(y,as.numeric(file[i,9]))
				x <- c(x,i)
			}
			else
				y[(i %% (96*7))+1] = y[(i %% (96*7))+1] + as.numeric(file[i,9])
		}

		y = y/3
		data <- matrix(c(x,y),ncol=2)

		#4. Starting the prediction
		Error = as.numeric(0.0)
		TotalFlow = as.numeric(0.0)
		model <- svm(y ~ x, data)
		predy <- predict(model,data)

		for(i in (96*7*3):((96*7*4)-1)){
			flow = as.numeric(file[i,9])
			pred = as.numeric(predy[(i %% (96*7))+1])
			TotalFlow = TotalFlow + flow
			Error = as.numeric(Error) + abs(flow - pred)
		}
		print( (100*Error)/TotalFlow )
		write( c(100*Error/TotalFlow ),file='SVM_error',ncolumns = 1,append = TRUE,sep = "")
	}
	else{
		print("> File doesn't exist.")
	}
}

run_all_SVM <- function(){
	all_files <- read.csv('../all_data/link-coordinates',header=F)
	for( i in 1:nrow(all_files)){
		filename = paste("","",collapse = "",sep = "")
		filename = paste("../all_data/",all_files[i,1],collapse = "",sep = "")
		if(file.exists(paste(filename,'Jan',collapse = "",sep = "")) && nrow(read.csv(paste(filename,'Mar',collapse = "",sep = ""))) == (2975)){
			print(paste("Start test - file :",paste(filename,'Jan',collapse = "",sep = "")))
			runtestSVM(paste(filename,'Jan',collapse = "",sep = ""))
		}
		if(file.exists(paste(filename,'Mar',collapse = "",sep = "")) && nrow(read.csv(paste(filename,'Mar',collapse = "",sep = ""))) == (2975)){
			print(paste("Start test - file :",paste(filename,'Mar',collapse = "",sep = "")))
			runtestSVM(paste(filename,'Mar',collapse = "",sep = ""))
		}
	}
}