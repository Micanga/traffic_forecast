library(forecast)

# GLOBAL VARIABLES ..................
N_TRAINING_DAYS <<- 3
N_POINTS <<- 5
WEIGHT <<- 0.60
DWEIGHT <<- 0.85
START_PERIOD <<- 1
END_PERIOD <<- 24
N_WEEK_DAYS <<- 7
START_DAY <<- 1
END_DAY <<- 30









# FORECASTING UTILS PACKAGE ....................
e_month <- function() {
    list(JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SET = 9, OUT = 10, NOV = 11, DEC = 12)
}

e_dayofweek <- function() {
    list(SUN = 1, MON = 2, TUE = 3, WED = 4, THU = 5, FRI = 6, SAT = 7)
}

isfeb <- function(month){
	if(month == e_month()$FEB)
		return(TRUE)
	return(FALSE)
}

outputfilename <- function(method,npredictions){
	name <- paste("","",collapse = "",sep = "")
	    
	name <- paste("Error",method,collapse = "",sep = "")
	name <- paste(name,as.character(npredictions),collapse = "",sep = "")

	return(name)
}

testfilename <- function(month,link_ref){
	name <- paste("","",collapse = "",sep = "")

        if(month == 1){
	    name <- paste("all_data/",link_ref,collapse = "",sep = "")
	    name <- paste(name,"Jan",collapse = "",sep = "")
	}
	else if(month == 2){
	    name <- paste("all_data/",link_ref,collapse = "",sep = "")
	    name <- paste(name,"Feb",collapse = "",sep = "")
	}
	else{
	     name <- paste("all_data/",link_ref,collapse = "",sep = "")
	     name <- paste(name,"Mar",collapse = "",sep = "")
        }
	
	return(name)
}

msdtrainning <- function(test_day, test_month, ntraining_days, month){
    startday = test_day
    while(ntraining_days > 0){
    	ntraining_days = ntraining_days-1

        startday = startday - 7
        if(startday <= 0){
			if(month == e_month()$FEB) startday = startday + 28
			else if(month %% 2 == 1) startday = startday + 31
			else startday = startday + 30

            if(test_month == month) return(1)
            else test_month = if(test_month == 0) e_month()$DEC else test_month-1
        }
    }
    if(test_month != month) return(35)
    else return(startday)
}





























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






























# ADAPTFLOW FORECASTING PACKAGE .................
createCoef <- function(){
	coef = list(day = c())
    for(i in 1:1){
        coef$day = c(coef$day,list(hour = c()))
        for(j in START_PERIOD:END_PERIOD){
	    	coef$day[i]$hour = c(coef$day[i]$hour,1.0)
        }
    }
    return(coef)
}


weightedtraining <- function(filename,ntrainning_days,weight){
	expected_train = ntrainning_days
	file = read.csv(filename)
    #1. Creating and initializing the support structures to make the training
    coef = createCoef()
    index = 1
        
    ntests = 0
    for( day in START_DAY:END_DAY){
    	#2. Vectorizing day flow
    	flow <- c()
		for( i in (START_PERIOD+(END_PERIOD*(day-1))):(END_PERIOD*day))
			flow <- c(flow,file[i,1])

        ntests = ntests + 1
                    
        Erro[index,1] = 0
        coef$day[index]$hour[1] = 1
        
        #3. TRAINING
        for(hour in (START_PERIOD+1):END_PERIOD){
            if(ntests == 1)
                bestCoef[index,hour] = 1+(flow[hour]-flow[hour-1])/flow[hour-1]
            else
	            bestCoef[index,hour] = weight*(1+(flow[hour]-flow[hour-1])/flow[hour-1]) + (1 - weight)*coef$day[index]$hour[hour]
                       
            coef$day[index]$hour[hour] = bestCoef[index,hour]
        }
        ntrainning_days = ntrainning_days - 1
    }

    #3. Validating the training
    if(ntests != expected_train)
    	return(NULL)

    return(coef)
}

adaptFlow <- function(filename,ntraining_days,npredictions,weight,npoints,dweight){
	file = read.csv(filename)
	#1. Calculating the coef
	coef = weightedtraining(filename,ntraining_days,weight)
	#print(coef)

	#2. Creating the hour vector (used in lsqr method)
    hour_vec = c()
    for(hour in START_PERIOD:END_PERIOD)
        hour_vec = c(hour_vec,hour)

    #3. Starting the variables and aux vectors
    index = 1
    day = 1

    f = c()
	for( i in START_PERIOD:(END_PERIOD*4) )
		f = c(f,as.numeric(file[i,1]))
    
    pf = c()
    for( i in START_PERIOD:END_PERIOD[])
		pf = c(pf,0.0)

	dtree = matrix(0.0,N_WEEK_DAYS,npoints)

    #4. initializing the variables
	Error = 0
	TotalFlow = 0
	errors = c()
	for( i in START_PERIOD:END_PERIOD)
		errors = c(errors,0.0)
            
	#5. running the test
	hour = START_PERIOD
	for( hour in START_PERIOD:(END_PERIOD-npredictions)){
		TotalFlow = TotalFlow + f[hour+npredictions-1]
        expected_result = f[hour+npredictions-1]

		if(hour != START_PERIOD){
            pf[hour-1] = f[hour-1]
            
            for(phour in 0:(npredictions-1)){
                if(phour+hour >= END_PERIOD)
                    break

        		#a. applying the least squares method to short term prediction
        		x <- c()
        		y <- c()
        		if(hour+phour == 2){
        			ls_next = as.numeric(pf[hour-1])
        		}
        		else if(hour+phour < npoints){
        			x = hour_vec[1:(hour+phour-1)]
        			y = pf[1:(hour+phour-1)]
        			fit = lsfit(x,y)
        			ls_next = (as.numeric(fit$coefficients[2])*(hour+phour))+as.numeric(fit$coefficients[1])
        		}
                else if(npoints == 0 || hour+phour == 24){
                    ls_next = as.numeric(pf[hour-1])	
                }
        		else{
        			x = hour_vec[(hour+phour-3):(hour+phour-1)]
        			y = pf[(hour+phour-3):(hour+phour-1)]
        			fit = lsfit(x,y)
        			ls_next = (as.numeric(fit$coefficients[2])*(hour+phour))+as.numeric(fit$coefficients[1])
        		}
                            
                #b. Calculating the forecasting point and choosing between the dynamic or static approach
                if(phour > 0 && hour > npoints){
                    expectedbehavior = 0
                    for(i in 1:npoints)
                        expectedbehavior = expectedbehavior + (dtree[index,i] >= 0.5)?(1):(-1)

                    if(expectedbehavior >= 0 && dweight < 0.5)
                        dweight = 1-dweight
                    else if(expectedbehavior < 0 && dweight >= 0.5)
                        dweight = 1-dweight

                    dweight = coef->day[index]$coef[hour+phour]*dweight
                    if(dweight > 1)
                        dweight = 1

                    for( i in 1:npoints)
                        dtree[index,npoints-i] = dtree[index,npoints-i-1]
                    dtree[index,1] = dweight
                }
                F = (dweight)*(as.numeric(pf[hour+phour-1])*as.numeric(coef$day[index]$hour[hour+phour])) + (1 - dweight)*(ls_next)

        		#c. Updating the prediction flows vector
                pf[hour+phour] = F
            }

            #d. Calculating the errors
            weigError = abs(expected_result - f[hour-1]*coef$day[index]$hour[hour+npredictions-1])
            lsqrError = abs(expected_result - ls_next)

            #e. Updating the dtree values
            for( i in 1:npoints)
                dtree[index,npoints-i] = dtree[index,npoints-i]
            dtree[index,1] = dweight

            #f. Updating the weight value to improove next prediction
            totalError = weigError + lsqrError
            if(weigError > lsqrError)
                dweight = 1 - (weigError/totalError)
            else
                dweight = (lsqrError/totalError)

            #g. Updating the hour 
            Error = Error + abs(expected_result - F)
            errors[hour+phour] = abs(expected_result - F)
		}    
  		else{
            errors[hour] = 0
        }
			    
	}

    #6. priting the errors in outputfile
    #print(100*Error/TotalFlow)
	#write(c(100*Error/TotalFlow),file=outputfilename("AdaptFlow",npredictions),ncolumns = 1,append = TRUE,sep = "")

	return(TRUE)
}

runtestAdaptFlow <- function(npredictions,filename){
	#1. Verifying the file existence
	if(file.exists(filename)){
		print(paste("Start test - file :",filename))
		sucess_flag = adaptFlow(filename,N_TRAINING_DAYS,npredictions,WEIGHT,N_POINTS,DWEIGHT)
	}
}
