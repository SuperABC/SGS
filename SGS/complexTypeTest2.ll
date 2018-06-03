
@printNum.constStr = constant [3 x i8] c"%d\00", align 1
@printStr.constStr = constant[3 x i8] c"%s\00", align 1
@printFloat.constStr = constant[3 x i8] c"%f\00", align 1
declare i32 @printf(i8*, ...)
declare i32 @scanf(i8*, ...)
declare i32 @putchar(i32)
declare i32 @getchar()
declare i8* @strcpy(i8*, i8*)

define i8 @intToChar(i32) {
	%2 = trunc i32 %0 to i8
	ret i8 %2
}

define i32 @charToInt(i8) {
	%2 = alloca i8, align 1
	store i8 %0, i8* %2, align 1
	%3 = load i8, i8* %2, align 1
	%4 = sext i8 %3 to i32
	ret i32 %4
}

define i1 @intToBool(i32) {
	%2 = icmp eq i32 %0, 0
	ret i1 %2
}

define i32 @boolToInt(i1) {
	%2 = sext i1 %0 to i32
	ret i32 %2
}

define float @intToFloat(i32) {
	%2 = sitofp i32 %0 to float
	ret float %2
}

define i32 @floatToInt(float) {
	%2 = fptosi float %0 to i32
	ret i32 %2
}

define i32 @printNum(i32) {
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32 %0)
	ret i32 0
}
define i32 @printFloat(float) {
	%2 = fpext float %0 to double
	%3 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printFloat.constStr, i32 0, i32 0), double %2)
	ret i32 0
}

define i32 @printStr(i8*) {
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)
	ret i32 0
}

define i32 @readNum() {
	%1 = alloca i32, align 4
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32* %1)
	%3 = load i32, i32* %1, align 4
	ret i32 %3
}
define i32 @newline() {
	call i32 @putchar(i32 10)
	ret i32 0
}

define i32 @readStr(i8*)  {
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)
	ret i32 0
}
; ModuleID = 'wtf'
source_filename = "wtf"

%Fucker = type { i32, [10 x i8], i32 }

@g1 = common global %Fucker zeroinitializer
@g2.array = common global [10 x %Fucker] zeroinitializer
@g2 = internal global %Fucker* getelementptr inbounds ([10 x %Fucker], [10 x %Fucker]* @g2.array, i32 0, i32 0)
@"12345.str" = private constant [6 x i8] c"12345\00"
@"432.str" = private constant [4 x i8] c"432\00"
@abcde.str = private constant [6 x i8] c"abcde\00"

define i32 @printFuckers(%Fucker*, i32) {
entry:
  %array.temp = alloca i8*
  %a = alloca %Fucker*
  store %Fucker* %0, %Fucker** %a
  %i = alloca i32
  store i32 %1, i32* %i
  %load = load i32, i32* %i
  %array.load = load %Fucker*, %Fucker** %a
  %visit = getelementptr inbounds %Fucker, %Fucker* %array.load, i32 %load
  %access.res = getelementptr inbounds %Fucker, %Fucker* %visit, i32 0, i32 1
  %2 = getelementptr inbounds [10 x i8], [10 x i8]* %access.res, i32 0, i32 0
  store i8* %2, i8** %array.temp
  %load1 = load i8*, i8** %array.temp
  %call.res = call i32 @printStr(i8* %load1)
  ret i32 %call.res
}

define i32 @main() {
entry:
  %array.temp8 = alloca i8*
  %array.temp5 = alloca i8*
  %array.temp1 = alloca i8*
  %array.temp = alloca i8*
  %fuckers.ptr = alloca %Fucker*
  %fuckers = alloca [10 x %Fucker]
  %0 = getelementptr inbounds [10 x %Fucker], [10 x %Fucker]* %fuckers, i32 0, i32 0
  store %Fucker* %0, %Fucker** %fuckers.ptr
  %array.load = load %Fucker*, %Fucker** %fuckers.ptr
  %visit = getelementptr inbounds %Fucker, %Fucker* %array.load, i32 0
  %access.res = getelementptr inbounds %Fucker, %Fucker* %visit, i32 0, i32 1
  %1 = getelementptr inbounds [10 x i8], [10 x i8]* %access.res, i32 0, i32 0
  store i8* %1, i8** %array.temp
  %str.ptr = load i8*, i8** %array.temp
  %2 = call i8* @strcpy(i8* %str.ptr, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @"12345.str", i32 0, i32 0))
  store i8* getelementptr inbounds (%Fucker, %Fucker* @g1, i32 0, i32 1, i32 0), i8** %array.temp1
  %str.ptr2 = load i8*, i8** %array.temp1
  %3 = call i8* @strcpy(i8* %str.ptr2, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @"432.str", i32 0, i32 0))
  %array.load3 = load %Fucker*, %Fucker** @g2
  %visit4 = getelementptr inbounds %Fucker, %Fucker* %array.load3, i32 2
  %access.res6 = getelementptr inbounds %Fucker, %Fucker* %visit4, i32 0, i32 1
  %4 = getelementptr inbounds [10 x i8], [10 x i8]* %access.res6, i32 0, i32 0
  store i8* %4, i8** %array.temp5
  %str.ptr7 = load i8*, i8** %array.temp5
  %5 = call i8* @strcpy(i8* %str.ptr7, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @abcde.str, i32 0, i32 0))
  %load = load %Fucker*, %Fucker** @g2
  %call.res = call i32 @printFuckers(%Fucker* %load, i32 2)
  store i8* getelementptr inbounds (%Fucker, %Fucker* @g1, i32 0, i32 1, i32 0), i8** %array.temp8
  %load9 = load i8*, i8** %array.temp8
  %call.res10 = call i32 @printStr(i8* %load9)
  %load11 = load %Fucker*, %Fucker** %fuckers.ptr
  %call.res12 = call i32 @printFuckers(%Fucker* %load11, i32 0)
  ret i32 0
}
