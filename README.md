# CC_Tool
<a href="https://colorcompass.jp/colorcompass/colorcompasmfa/"><img height="200" src="https://colorcompass.jp/wp-content/uploads/2022/06/cropped-logo.png"/></a>
<details>
<summary align="center">=============================================================</summary>


Thư viện MICROSOFT FOUNDATION CLASS: CC_Tool Tổng quan về dự án
=======================================================================================

CC_Tool được tạo bởi Trình hướng dẫn ứng dụng thể hiện cách sử dụng cơ bản
của các Lớp nền tảng của Microsoft. Vui lòng sử dụng nó làm mẫu để tạo ứng dụng.

Bài viết này chứa mô tả ngắn gọn về từng tệp tạo nên ứng dụng CC_Tool.

CC_Tool.vcproj
    Đây là tệp dự án chính cho dự án VC++ được tạo bởi Trình hướng dẫn ứng dụng.
    Chứa thông tin phiên bản của Visual C++ nơi tệp được tạo.
    Nó cũng bao gồm thông tin về nền tảng,cấu hình và 
    các tính năng của dự án được chọn trong Trình hướng dẫn ứng dụng.

CC_Tool.h
    Đây là tệp tiêu đề chính cho ứng dụng của bạn. 
    Tệp này chứa các tiêu đề dành riêng cho dự án khác, 
    bao gồm Resource.h, khai báo lớp ứng dụng CCC_ToolApp.

CC_Tool.cpp
    Đây là tệp nguồn ứng dụng chính chứa lớp ứng dụng CCC_ToolApp.

CC_Tool.rc
    Đây là tệp liệt kê các tài nguyên Microsoft Windows được chương trình sử dụng. 
    Tệp này chứa các biểu tượng, bitmap và con trỏ được lưu trữ trong thư mục con RES. 
    Tập tin này có thể được chỉnh sửa trực tiếp trong Microsoft Visual C++. 
    Tài nguyên dự án có thể được tìm thấy tại 1041.

res\CC_Tool.ico
    Đây là một tập tin biểu tượng được sử dụng làm biểu tượng của ứng dụng. 
    Biểu tượng này được bao gồm trong tệp tài nguyên chính CC_Tool.rc.

res\CC_Tool.rc2
    Tệp này chứa các tài nguyên đã được chỉnh sửa bên ngoài Microsoft Visual C++. 
    Đặt bất kỳ tài nguyên nào không thể chỉnh sửa được trong trình chỉnh sửa tài nguyên trong tệp này.
CC_Tool.reg
    Tệp đăng ký này là một tệp mẫu trình bày cách định cấu hình đăng ký cho khung. 
    Sử dụng nó dưới dạng tệp .reg với ứng dụng của bạn hoặc xóa nó 
    và sử dụng RegisterShellFileTypes đăng ký mặc định.


////////////////////////////////////////////////////////////////////////////////////////////

Cửa sổ khung chính:
     Dự án chứa các giao diện MFC tiêu chuẩn.

MainFrm.h, MainFrm.cpp
    Các tệp này chứa lớp khung CMainFrame.
    Lớp khung bắt nguồn từ CFrameWnd 
    và kiểm soát tất cả chức năng của khung SDI.

res\Thanh công cụ.bmp
    Tệp bitmap này sẽ được sử dụng làm hình ảnh ô thanh công cụ. 
    Thanh công cụ và thanh trạng thái ban đầu được tạo trong lớp CMainFrame. 
    Để thêm nút thanh công cụ, hãy chỉnh sửa bitmap thanh công cụ này bằng Trình chỉnh sửa tài nguyên 
    và cập nhật mảng Thanh công cụ IDR_MAINFRAME trong tệp CC_Tool.rc.

////////////////////////////////////////////////////////////////////////////////////////////

Trình hướng dẫn ứng dụng tạo một loại tài liệu và một dạng xem:

CC_ToolDoc.h, CC_ToolDoc.cpp - Tài liệu Các tệp này chứa lớp CCC_ToolDoc. 
Chỉnh sửa các tệp này để thêm dữ liệu tài liệu đặc biệt, 
lưu tệp và tải (thông qua CCC_ToolDoc::Serialize).

     Tài liệu chứa chuỗi sau:
         Phần mở rộng tập tin: cctd
         ID loại tệp: CCTool.Document
         Chú thích khung chính: CC_Tool
         Tên loại tài liệu: CC_Tool
         Tên bộ lọc: Tệp CC_Tool (*.cctd)
         Tên viết tắt mới cho tệp: CC_Tool
         Tên dài của loại tệp: CC_Tool.Document

CC_ToolView.h, CC_ToolView.cpp - Chế độ xem tài liệu Những tệp này chứa lớp CCC_ToolView. 
Đối tượng CCC_ToolView được sử dụng để hiển thị đối tượng CCC_ToolDoc.



////////////////////////////////////////////////////////////////////////////////////////////

Các tính năng khác:

Điều khiển ActiveX
    Ứng dụng này bao gồm hỗ trợ sử dụng các điều khiển ActiveX.

////////////////////////////////////////////////////////////////////////////////////////////

Các tập tin tiêu chuẩn khác:

StdAfx.h, StdAfx.cpp
    Các tệp này được sử dụng để xây dựng tệp tiêu đề được biên dịch trước (PCH) CC_Tool.pch 
    và tệp loại được biên dịch trước StdAfx.obj.

Resource.h
    Đây là tệp tiêu đề tiêu chuẩn xác định ID tài nguyên mới. Microsoft Visual C++ đọc và cập nhật tệp này.

CC_Tool.manifest
    Các tệp kê khai ứng dụng được sử dụng trong Windows XP để mô tả sự phụ thuộc của ứng dụng vào các phiên bản cụ thể của các tập hợp song song. 
    Trình tải sử dụng thông tin này để tải tập hợp thích hợp từ bộ nhớ đệm của tập hợp hoặc tập hợp riêng từ ứng dụng. 
    Tệp kê khai ứng dụng được đưa vào để phân phối lại dưới dạng tệp .manifest bên ngoài được cài đặt trong cùng thư mục với ứng dụng thực thi hoặc được đưa vào tệp thực thi dưới dạng tài nguyên.
////////////////////////////////////////////////////////////////////////////////////////////

Các ghi chú khác:

Trình hướng dẫn ứng dụng sử dụng các nhận xét bắt đầu bằng "TODO:" để chỉ ra các phần của mã nguồn mà bạn cần thêm hoặc tùy chỉnh.

Nếu ứng dụng của bạn sử dụng MFC trong một DLL được chia sẻ, bạn phải phân phối lại MFC DLL. 
Ngoài ra, nếu ứng dụng của bạn sử dụng ngôn ngữ không phải là ngôn ngữ của hệ điều hành, bạn cũng phải phân phối lại tài nguyên được bản địa hóa tương ứng MFC90XXX.DLL. Để biết thêm thông tin về các chủ đề này, hãy xem phần Phân phối lại ứng dụng Visual C++ của tài liệu MSDN.

////////////////////////////////////////////////////////////////////////////////////////////


lịch sử:
Ngày 12 tháng 4 năm 2023
                             Sửa đổi đáng kể kế hoạch đo lường.
                                 1. Cho phép đặt phạm vi thời gian đo.
                                 2. Đồng thời khi đo có thể lưu vào file kèm theo ghi chú bổ sung.
                                 3. Đặt phiên bản thành 1.5.
Ngày 20 tháng 4 năm 2023
                             Khắc phục sự cố trong đó tệp được tạo mỗi khi ngày bị vượt qua.
                             Đặt phiên bản thành 1.6.

Ngày 2 tháng 9 năm 2023 (hiện tại chưa thêm vào code)
                             Thêm khả năng hiển thị và lưu các hệ số bước sóng, giá trị bước sóng, độ tối, độ lợi, v.v.

</details>
