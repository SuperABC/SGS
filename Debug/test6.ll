@printNum.constStr = constant [3 x i8] c"%d\00", align 1
@printStr.constStr = constant[3 x i8] c"%s\00", align 1
@printFloat.constStr = constant[3 x i8] c"%f\00", align 1
declare i32 @printf(i8*, ...)
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

define i32 @newline() {
	call i32 @putchar(i32 10)
	ret i32 0
}
; ModuleID = 'wtf'
source_filename = "wtf"

%list = type { i32, [10 x i32] }

@one = common global %list zeroinitializer
@lists.array = common global [10 x %list] zeroinitializer
@lists = internal global %list* getelementptr inbounds ([10 x %list], [10 x %list]* @lists.array, i32 0, i32 0)

define i32 @main() {
entry:
  %array.temp11 = alloca i32*
  %array.temp6 = alloca i32*
  %array.temp1 = alloca i32*
  %array.temp = alloca i32*
  store i32* getelementptr inbounds (%list, %list* @one, i32 0, i32 1, i32 0), i32** %array.temp
  %array.load = load i32*, i32** %array.temp
  %visit = getelementptr inbounds i32, i32* %array.load, i32 0
  store i32 1, i32* %visit
  store i32* getelementptr inbounds (%list, %list* @one, i32 0, i32 1, i32 0), i32** %array.temp1
  %array.load2 = load i32*, i32** %array.temp1
  %visit3 = getelementptr inbounds i32, i32* %array.load2, i32 0
  %load = load i32, i32* %visit3
  %call.res = call i32 @printNum(i32 %load)
  %array.load4 = load %list*, %list** @lists
  %visit5 = getelementptr inbounds %list, %list* %array.load4, i32 0
  %access.res = getelementptr inbounds %list, %list* %visit5, i32 0, i32 1
  %0 = getelementptr inbounds [10 x i32], [10 x i32]* %access.res, i32 0, i32 0
  store i32* %0, i32** %array.temp6
  %array.load7 = load i32*, i32** %array.temp6
  %visit8 = getelementptr inbounds i32, i32* %array.load7, i32 0
  store i32 2, i32* %visit8
  %array.load9 = load %list*, %list** @lists
  %visit10 = getelementptr inbounds %list, %list* %array.load9, i32 0
  %access.res12 = getelementptr inbounds %list, %list* %visit10, i32 0, i32 1
  %1 = getelementptr inbounds [10 x i32], [10 x i32]* %access.res12, i32 0, i32 0
  store i32* %1, i32** %array.temp11
  %array.load13 = load i32*, i32** %array.temp11
  %visit14 = getelementptr inbounds i32, i32* %array.load13, i32 0
  %load15 = load i32, i32* %visit14
  %call.res16 = call i32 @printNum(i32 %load15)
  ret i32 0
}
