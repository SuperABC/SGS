
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

@a = common global i32 0
@arr.array = common global [10 x i32] zeroinitializer
@arr = internal global i32* getelementptr inbounds ([10 x i32], [10 x i32]* @arr.array, i32 0, i32 0)

declare i32 @visitGArray(i32)

define i32 @visitGArray.1(i32) {
entry:
  %result = alloca i32
  %i = alloca i32
  store i32 %0, i32* %i
  %load = load i32, i32* %i
  %array.load = load i32*, i32** @arr
  %visit = getelementptr inbounds i32, i32* %array.load, i32 %load
  %load.val = load i32, i32* %visit
  store i32 %load.val, i32* %result
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

declare i32 @writeGArray(i32*, i32, i32)

define i32 @writeGArray.2(i32*, i32, i32) {
entry:
  %result = alloca i32
  %b = alloca i32*
  store i32* %0, i32** %b
  %i = alloca i32
  store i32 %1, i32* %i
  %v = alloca i32
  store i32 %2, i32* %v
  %load = load i32, i32* %i
  %array.load = load i32*, i32** %b
  %visit = getelementptr inbounds i32, i32* %array.load, i32 %load
  %load.val = load i32, i32* %v
  store i32 %load.val, i32* %visit
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @main() {
entry:
  %load = load i32*, i32** @arr
  %call.res = call i32 @writeGArray.2(i32* %load, i32 1, i32 2)
  %call.res1 = call i32 @visitGArray.1(i32 1)
  %call.res2 = call i32 @printNum(i32 %call.res1)
  ret i32 0
}
