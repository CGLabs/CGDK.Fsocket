# CGDK.Fsocket<br>
network classes for unreal 3D Fsocket with CGDK::buffer<br>

### CGCK10.Fsocket란?<br>
   CGDK10에서 제공하는 unreal 3d용 client socket 모듈입니다.<br>
<br>

### 특징<br>
   CGDK10.Fsocket은 unreal 3d에서 제공하는 Fsocket을 사용하여<br>
   CGDK10으로 제작된 서버와 손쉬운 통신과 CGDK10과 동일한 방식으로 코딩이 가능하도록 하기 위해 랩핑(Wrapping)한 클래스들입니다.<br>
   헤더 파일로만 제작되어 library의 링크 없이 헤더 파일을 include만으로 가볍게 사용가능 합니다.<br>
   이로 인해 플랫폼 종속성이 없어 unreal 3d가 동작하는 모든 플랫폼에서 사용 가능합니다.<br>
   메시지 송수신용 버퍼로 CGDK.buffer를 사용하고 있어 CGDK::buffer를 포함하고 있습니다.

## 사용법<br>
   CGDK10.Fsocket을 사용하는 방법은 대충 아래와 같습니다.<br>
<br>
   __1.__ "/cgdk" 폴더 전체를 본인이 만드신 unreal 3d 폴더에 통체로 복사해 넣습니다.<br>
   __2.__ "cgdk/sdk10/net.socket.h"만 include 해주시면 됩니다.<br>

   ``` C++
      #include "cgdk/sdk10/net.socket.h"<br>
   ```
<br>

   __3.__ Fsocket을 사용하므로 Sockets당 모듈이 link되도록 설정해 주어야 합니다.<br>
      따라서 PublicDependencyModuleNames에 "Sockets" 를 추가해 주십시요.<br>
   ``` C#
      PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", ... , "Sockets" });
   ```
<br>

   __4.__ exception 컴파일 옵션을 설정해 줍니다.<br>
      CGDK는 exception을 사용하므로 이에 따른 옵션은 설정해 주어야 합니다.<br>
      따라서 프로젝트명.build.cs 파일을 열어 아래와 같은 옵션을 추가해 주십시요.<br>
   ``` C#
      bEnableExceptions = true;
   ```
<br>

   __5.__ 성공적으로 컴파일 되는지 확인하십시요.<br>
      성공적으로 컴파일 된다면 이제 CGDK10.Fsocket의 socket들을 사용하실수 있습니다.<br>
<br>

### 주의사항

__1.__ socket 관련 i/o를 처리하기 위해 unreal 3d의 WorldSubSystem(UTickableWorldSubsystem 를 사용하고 있습니다.<br>
   World Tick이 호출될 때 World Tick과 동기화 되어 호출이 되므로 별도의 쓰레드는 사용하고 있지 않습니다.<br>
   따라서 쓰레드 안전 처리는 특별히 필요하지 않으며 World Tick은 네크워크 지연 시간에 큰 영향을 미칩니다.<br>

__2.__ CGDK.buffer (https://github.com/CGLabs/CGDK.buffer/blob/master/LICENSE)을 포함하고 있습니다.
<br>

### 라이센스(License)
   MIT License(IT) 를 따르고 있습니다.

