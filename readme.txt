***************************************************************************
                                                                           
                      Cho sanghyun's Game Classes II                       
              Ver 10.0 for Unreal Engine / Release 2019.12.11              
                                                                           
                                  Fsocket
                                                                           
                                                                           
                                                                           
                                                                           
  This Program is programmed by Cho SangHyun. sangduck@cgcii.co.kr         
  Best for Game Developement and Optimized for Game Developement.          
                                                                           
                (c) 2003. Cho Sanghyun. All right reserved.                
                          http://www.CGCII.co.kr                           
                                                                           
***************************************************************************

1. CGCK10.Fsocket
   CGDK10에서 제공하는 unreal 3d용 client socket 모듈입니다.

2. 특징
   CGDK10.Fsocket은 자체적인 소켓을 구현하지 않았으며 unreal 3d에서 제공하는 Fsocket을 사용하여 구현하였습니다.
   CGDK10.Fsocket은 CGDK10과 동일한 방식으로 코딩이 가능하도록 하기 위해 랩핑(Wrapping)한 라이브러리로 되어 헤더 파일만으로 제작되어 include만으로 가볍게 사용가능합니다.
   CGDK10와는 모듈상의 의존 관계가 없이 독립적으로 사용 가능하며 플랫폼에도 구해받지 않고 사용 가능합니다.
   또한 가볍게 제공하기 위해 CGDK10 중 기본적인 소켓 기능을 Fsocket을 사용해 구현했습니다.
   직렬화는 CGDK.buffer로 수행합니다.

3. 사용법
   CGDK10.Fsocket을 사용하는 방법은 대충 아래와 같습니다.

   1) "/cgdk" 폴더 전체를 본인이 만드신 unreal 3d 폴더에 통체로 복사해 넣습니다.
   2) "cgdk/sdk10/net.socket.h"만 include 해주시면 됩니다.
   
      #include "cgdk/sdk10/net.socket.h"

   3) Fsocket을 사용하므로 Sockets당 모듈이 link되도록 설정해 주어야 합니다.
      따라서 PublicDependencyModuleNames에 "Sockets" 를 추가해 주십시요.

      PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", ... , "Sockets" });


   4) exception 컴파일 옵션을 설정해 줍니다.
      CGDK는 exception을 사용하므로 이에 따른 옵션은 설정해 주어야 합니다.
      따라서 프로젝트명.build.cs 파일을 열어 아래와 같은 옵션을 추가해 주십시요.

      bEnableExceptions = true;


   5) 성공적으로 컴파일 되는지 확인하십시요.
      성공적으로 컴파일 된다면 이제 CGDK10.Fsocket의 socket들을 사용하실수 있습니다.


3. 주의사항
    1) socket 관련 i/o를 처리하기 위해 unreal 3d의 WorldSubSystem(UTickableWorldSubsystem 를 사용하고 있습니다.
       World Tick이 호출될 때 World Tick과 동기화 되어 호출이 되므로 별도의 쓰레드는 사용하고 있지 않습니다.
       따라서 쓰레드 안전 처리는 특별히 필요하지 않습니다.
       이에 따라 네크워크 지연 시간은 World Tick에 달려 있습니다.

    2) CGDK.buffer (https://github.com/CGLabs/CGDK.buffer/blob/master/LICENSE)을 포함하고 있습니다.


4. 라이센스(License)
   MIT License(IT) 를 따르고 있습니다.

CGDK.Fsocket ()
----------------------------------------
The MIT License (MIT)

Copyright (c) 2022 Cho SangHyun.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and
/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDE-
RS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
 OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------


CGDK.buffer (https://github.com/CGLabs/CGDK.buffer/blob/master/LICENSE)
----------------------------------------
The MIT License (MIT)

Copyright (c) 2015 Cho SangHyun.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and
/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDE-
RS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
 OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------
