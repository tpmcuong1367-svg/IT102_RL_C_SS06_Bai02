#include <stdio.h>
#include <string.h>

#define MAX 100
#define HAN_MUON 14
#define DON_GIA_PHAT 5000

int main(void)
{
    char maPhieu[MAX][15];
    char tenSach[MAX][50];
    int soNgayMuon[MAX];
    int tienPhat[MAX];
    int soLuong = 0;

    int luaChon;
    int c;
    int i, k;

    do
    {
        printf("\n================ HỆ THỐNG QUẢN LÝ MƯỢN TRẢ SÁCH (LIBRARY_WMS) ================\n");
        printf("1. Thêm lượt mượn sách mới\n");
        printf("2. Hiển thị danh sách mượn sách\n");
        printf("3. Cập nhật số ngày mượn theo Mã phiếu\n");
        printf("4. Xóa lượt mượn sách theo Mã phiếu\n");
        printf("5. Thống kê danh sách phiếu mượn bị phạt (Quá hạn > 14 ngày)\n");
        printf("6. Thoát chương trình\n");
        printf("==============================================================================\n");
        printf("Lựa chọn của bạn: ");

        if (scanf("%d", &luaChon) != 1)
        {
            luaChon = -1; /* nhập không phải số */
        }
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        switch (luaChon)
        {
        /* ---------------- 1. THÊM MỚI ---------------- */
        case 1:
            printf("\n=== THÊM LƯỢT MƯỢN SÁCH MỚI ===\n");
            if (soLuong >= MAX)
            {
                printf("Hệ thống đầy, không thể thêm phiếu mượn mới!\n");
                break;
            }
            else
            {
                char maMoi[15];
                int trung;
                int ngay;
                int len;

                /* Nhập mã phiếu, kiểm tra trùng bằng tìm kiếm tuyến tính */
                do
                {
                    trung = 0;
                    printf("Nhập mã phiếu mượn: ");
                    if (scanf("%14s", maMoi) != 1)
                    {
                        maMoi[0] = '\0';
                    }
                    while ((c = getchar()) != '\n' && c != EOF)
                        ;

                    for (i = 0; i < soLuong; i++)
                    {
                        if (strcmp(maPhieu[i], maMoi) == 0)
                        {
                            trung = 1;
                            break;
                        }
                    }
                    if (trung)
                    {
                        printf("Mã phiếu '%s' đã tồn tại, vui lòng nhập lại!\n", maMoi);
                    }
                    else if (maMoi[0] == '\0')
                    {
                        printf("Mã phiếu không hợp lệ, vui lòng nhập lại!\n");
                        trung = 1;
                    }
                } while (trung);
                strcpy(maPhieu[soLuong], maMoi);

                /* Nhập tên sách (có khoảng trắng) */
                printf("Nhập tên sách: ");
                if (fgets(tenSach[soLuong], 50, stdin) == NULL)
                {
                    tenSach[soLuong][0] = '\0';
                }
                len = (int)strlen(tenSach[soLuong]);
                if (len > 0 && tenSach[soLuong][len - 1] == '\n')
                {
                    tenSach[soLuong][len - 1] = '\0';
                }
                else
                {
                    /* chuỗi dài hơn bộ đệm: bỏ phần còn thừa */
                    while ((c = getchar()) != '\n' && c != EOF)
                        ;
                }

                /* Nhập số ngày mượn > 0 */
                do
                {
                    printf("Nhập số ngày mượn thực tế: ");
                    if (scanf("%d", &ngay) != 1)
                    {
                        ngay = 0;
                    }
                    while ((c = getchar()) != '\n' && c != EOF)
                        ;
                    if (ngay <= 0)
                    {
                        printf("Số ngày mượn phải là số nguyên lớn hơn 0, vui lòng nhập lại!\n");
                    }
                } while (ngay <= 0);
                soNgayMuon[soLuong] = ngay;

                tienPhat[soLuong] = (soNgayMuon[soLuong] > HAN_MUON)
                                        ? (soNgayMuon[soLuong] - HAN_MUON) * DON_GIA_PHAT
                                        : 0;

                if (soNgayMuon[soLuong] > HAN_MUON)
                {
                    printf("--> Tiền phạt tự động tính: %d VNĐ ((%d - %d) * %d)\n",
                           tienPhat[soLuong], soNgayMuon[soLuong], HAN_MUON, DON_GIA_PHAT);
                }
                else
                {
                    printf("--> Tiền phạt tự động tính: 0 VNĐ\n");
                }

                soLuong++;
                printf("--> Thêm lượt mượn sách thành công!\n");
            }
            break;

        /* ---------------- 2. HIỂN THỊ ---------------- */
        case 2:
            if (soLuong == 0)
            {
                printf("Danh sách mượn sách hiện đang rỗng!\n");
            }
            else
            {
                char dinhDang[MAX + 1][20]; /* tiền phạt có dấu chấm ngăn cách; phần tử cuối = tổng */
                long long tong = 0;

                for (i = 0; i < soLuong; i++)
                {
                    tong += tienPhat[i];
                }

                for (k = 0; k <= soLuong; k++)
                {
                    long long giaTri = (k < soLuong) ? tienPhat[k] : tong;
                    char tho[24];
                    int n, j, pos = 0;

                    sprintf(tho, "%lld", giaTri);
                    n = (int)strlen(tho);
                    for (j = 0; j < n; j++)
                    {
                        if (j > 0 && (n - j) % 3 == 0)
                        {
                            dinhDang[k][pos++] = '.';
                        }
                        dinhDang[k][pos++] = tho[j];
                    }
                    dinhDang[k][pos] = '\0';
                }

                printf("\n================================ DANH SÁCH MƯỢN SÁCH ================================\n");
                printf("%-4s | %-10s | %-30s | %-8s | %-15s\n",
                       "STT", "Mã Phiếu", "Tên Sách", "Số Ngày", "Tiền Phạt (VNĐ)");
                printf("-------------------------------------------------------------------------------------\n");
                for (i = 0; i < soLuong; i++)
                {
                    printf("%-4d | %-10s | %-30s | %-8d | %-15s\n",
                           i + 1, maPhieu[i], tenSach[i], soNgayMuon[i], dinhDang[i]);
                }
                printf("-------------------------------------------------------------------------------------\n");
                printf("Tổng tiền phạt toàn hệ thống: %s VNĐ\n", dinhDang[soLuong]);
            }
            break;

        /* ---------------- 3. CẬP NHẬT ---------------- */
        case 3:
            printf("\n=== CẬP NHẬT SỐ NGÀY MƯỢN ===\n");
            {
                char maTim[15];
                int viTri = -1;
                int ngayMoi;

                printf("Nhập mã phiếu mượn cần cập nhật: ");
                if (scanf("%14s", maTim) != 1)
                {
                    maTim[0] = '\0';
                }
                while ((c = getchar()) != '\n' && c != EOF)
                    ;

                for (i = 0; i < soLuong; i++)
                {
                    if (strcmp(maPhieu[i], maTim) == 0)
                    {
                        viTri = i;
                        break;
                    }
                }

                if (viTri == -1)
                {
                    printf("Không tìm thấy phiếu mượn có mã: %s\n", maTim);
                }
                else
                {
                    printf("Tìm thấy phiếu mượn của sách: %s (Số ngày hiện tại: %d)\n",
                           tenSach[viTri], soNgayMuon[viTri]);
                    do
                    {
                        printf("Nhập số ngày mượn thực tế mới: ");
                        if (scanf("%d", &ngayMoi) != 1)
                        {
                            ngayMoi = 0;
                        }
                        while ((c = getchar()) != '\n' && c != EOF)
                            ;
                        if (ngayMoi <= 0)
                        {
                            printf("Số ngày mượn phải là số nguyên lớn hơn 0, vui lòng nhập lại!\n");
                        }
                    } while (ngayMoi <= 0);

                    soNgayMuon[viTri] = ngayMoi;
                    tienPhat[viTri] = (ngayMoi > HAN_MUON) ? (ngayMoi - HAN_MUON) * DON_GIA_PHAT : 0;
                    printf("--> Tiền phạt mới: %d VNĐ\n", tienPhat[viTri]);
                    printf("--> Cập nhật số ngày mượn thành công!\n");
                }
            }
            break;

        /* ---------------- 4. XÓA ---------------- */
        case 4:
            printf("\n=== XÓA LƯỢT MƯỢN SÁCH ===\n");
            {
                char maXoa[15];
                int viTri = -1;

                printf("Nhập mã phiếu mượn cần xóa: ");
                if (scanf("%14s", maXoa) != 1)
                {
                    maXoa[0] = '\0';
                }
                while ((c = getchar()) != '\n' && c != EOF)
                    ;

                for (i = 0; i < soLuong; i++)
                {
                    if (strcmp(maPhieu[i], maXoa) == 0)
                    {
                        viTri = i;
                        break;
                    }
                }

                if (viTri == -1)
                {
                    printf("Không tìm thấy phiếu mượn có mã: %s\n", maXoa);
                }
                else
                {
                    /* Dồn mảng từ viTri đến soLuong - 2 */
                    for (i = viTri; i < soLuong - 1; i++)
                    {
                        strcpy(maPhieu[i], maPhieu[i + 1]);
                        strcpy(tenSach[i], tenSach[i + 1]);
                        soNgayMuon[i] = soNgayMuon[i + 1];
                        tienPhat[i] = tienPhat[i + 1];
                    }
                    soLuong--;
                    printf("Xóa phiếu mượn thành công!\n");
                }
            }
            break;

        /* ---------------- 5. THỐNG KÊ QUÁ HẠN ---------------- */
        case 5:
            printf("\n=== DANH SÁCH PHIẾU MƯỢN BỊ PHẠT (QUÁ HẠN > %d NGÀY) ===\n", HAN_MUON);
            {
                int dem = 0;

                for (i = 0; i < soLuong; i++)
                {
                    if (soNgayMuon[i] > HAN_MUON)
                    {
                        if (dem == 0)
                        {
                            printf("%-4s | %-10s | %-30s | %-8s | %-15s\n",
                                   "STT", "Mã Phiếu", "Tên Sách", "Số Ngày", "Tiền Phạt (VNĐ)");
                            printf("-------------------------------------------------------------------------------------\n");
                        }
                        dem++;
                        printf("%-4d | %-10s | %-30s | %-8d | %-15d\n",
                               dem, maPhieu[i], tenSach[i], soNgayMuon[i], tienPhat[i]);
                    }
                }
                if (dem == 0)
                {
                    printf("Không có lượt mượn nào bị quá hạn phạt!\n");
                }
            }
            break;

        /* ---------------- 6. THOÁT ---------------- */
        case 6:
            printf("Cảm ơn bạn đã sử dụng LIBRARY_WMS. Tạm biệt!\n");
            break;

        default:
            printf("Lựa chọn không hợp lệ. Vui lòng thử lại!\n");
            break;
        }
    } while (luaChon != 6);

    return 0;
}
