#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *bloodInventoryFiles[] =
    {
        "D:\\project_blood_management\\blood_types\\add_a+.txt",
        "D:\\project_blood_management\\blood_types\\add_a-.txt",
        "D:\\project_blood_management\\blood_types\\add_b+.txt",
        "D:\\project_blood_management\\blood_types\\add_b-.txt",
        "D:\\project_blood_management\\blood_types\\add_ab+.txt",
        "D:\\project_blood_management\\blood_types\\add_ab-.txt",
        "D:\\project_blood_management\\blood_types\\add_o+.txt",
        "D:\\project_blood_management\\blood_types\\add_o-.txt"};

struct Date
{
    int day;
    int month;
    int year;
};

int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

struct Date calculateExpiryDate(int day, int month, int year)
{
    int datesInMonth[] = {31, 28, 31, 30, 31, 30,
                          31, 31, 30, 31, 30, 31};

    day += 40;

    while (1)
    {
        datesInMonth[1] = isLeapYear(year) ? 29 : 28;

        if (day <= datesInMonth[month - 1])
        {
            break;
        }

        day -= datesInMonth[month - 1];
        month++;

        if (month > 12)
        {
            month = 1;
            year++;
        }
    }

    struct Date result = {day, month, year};
    return result;
}

void adjustInventoryManually()
{
    while (1)
    {
        int choice, id, unit, day, month, year;
        char donorNote[100];

        printf("\n=== MANUAL BLOOD INVENTORY ADJUSTMENT ===\n");
        printf("1.A+  2.A-  3.B+  4.B-  5.AB+  6.AB-  7.O+  8.O-\n");
        printf("Select Blood Group to update (0 to exit): ");
        scanf("%d", &choice);

        if (choice == 0)
            break;
        if (choice < 1 || choice > 8)
        {
            printf("Invalid Choice. Try again.\n");
            continue;
        }

        printf("Enter Donor ID or Reference ID: ");
        scanf("%d", &id);

        printf("Enter Donation Date (dd-mm-yyyy): ");
        scanf("%d-%d-%d", &day, &month, &year);

        struct Date expiry = calculateExpiryDate(day, month, year);
        printf("Calculated Expiration Date: %02d-%02d-%d\n", expiry.day, expiry.month, expiry.year);

        printf("Enter Blood Unit in ml (450 or 500): ");
        scanf("%d", &unit);
        getchar();

        printf("Optional Note (e.g., repeat donor, emergency stock): ");
        fgets(donorNote, sizeof(donorNote), stdin);
        donorNote[strcspn(donorNote, "\n")] = '\0';

        FILE *add = fopen(bloodInventoryFiles[choice - 1], "a+");
        if (add == NULL)
        {
            printf("Error opening inventory file!\n");
            continue;
        }

        fprintf(add, "Id: %d, Date: %02d-%02d-%d, Unit: %d ml, Expiration: %02d-%02d-%d, Note: %s\n",
                id, day, month, year, unit, expiry.day, expiry.month, expiry.year, donorNote);

        fclose(add);
        printf("Blood unit manually added to %s inventory.\n", bloodInventoryFiles[choice - 1]);
    }
}

void displaySingleBloodGroupInventory()
{
    int choice;
    char buffer[200];
    while (1)
    {
        printf("\n1.A+ 2.A- 3.B+ 4.B- 5.AB+ 6.AB- 7.O+ 8.O- \n");
        printf("Select Blood Unit from above to display(0 to exit): ");
        scanf("%d", &choice);
        if (choice == 0)
            return;
        if (choice < 1 || choice > 8)
        {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        FILE *show = fopen(bloodInventoryFiles[choice - 1], "r");
        if (show == NULL)
        {
            printf("Error opening file!\n");
            return;
        }
        printf("\n---     Blood Unit Details     ---\n");
        while (fgets(buffer, sizeof(buffer), show))
        {
            printf("%s", buffer);
        }
        fclose(show);
        return;
    }
}

void displayFullInventoryChart()
{
    const char *bloodGroups[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};

    char buffer[200];
    printf("\n========== FULL BLOOD INVENTORY CHART ==========\n");

    for (int i = 0; i < 8; i++)
    {
        FILE *show = fopen(bloodInventoryFiles[i], "r");
        if (show == NULL)
        {
            printf("\n[%s] - Error opening file or no data available.\n", bloodGroups[i]);
            continue;
        }

        printf("\n--- Blood Group: %s ---\n", bloodGroups[i]);
        while (fgets(buffer, sizeof(buffer), show))
        {
            printf("%s", buffer);
        }
        fclose(show);
    }

    printf("\n===============================================\n");
}

int isBloodAvailableForGroup(int choice)
{
    FILE *file = fopen(bloodInventoryFiles[choice - 1], "r");
    if (file == NULL)
    {
        printf("No inventory file found for %s.\n", bloodInventoryFiles[choice - 1]);
        return 0;
    }

    char buffer[200];
    int available = 0;

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strstr(buffer, "Id:") != NULL)
        {
            available = 1;
            break;
        }
    }

    fclose(file);
    return available;
}

struct Person
{
    int id;
    char name[50];
    char bloodGroup[10];
    int age;
    char phone[15];
};

void registerNewDonor()
{
    FILE *f;
    struct Person p;
    int unit, day, month, year, index = -1;

    f = fopen("D:\\project_blood_management\\Blood_donation\\donors.txt", "a");
    if (f == NULL)
    {
        printf("Error opening donor file.\n");
        return;
    }

    printf("Enter Donor ID: ");
    scanf("%d", &p.id);
    getchar();

    printf("Enter Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &p.age);
    getchar();

    printf("Enter Blood Group (e.g., A+, O-): ");
    fgets(p.bloodGroup, sizeof(p.bloodGroup), stdin);
    p.bloodGroup[strcspn(p.bloodGroup, "\n")] = '\0';

    printf("Enter Phone: ");
    fgets(p.phone, sizeof(p.phone), stdin);
    p.phone[strcspn(p.phone, "\n")] = '\0';

    fprintf(f, "%d,%s,%d,%s,%s\n", p.id, p.name, p.age, p.bloodGroup, p.phone);
    fclose(f);
    printf("Donor registered successfully.\n");

    const char *bloodGroups[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    for (int i = 0; i < 8; i++)
    {
        if (strcmp(p.bloodGroup, bloodGroups[i]) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Invalid blood group. Skipping blood unit entry.\n");
        return;
    }

    printf("Enter Donation Date (dd-mm-yyyy): ");
    scanf("%d-%d-%d", &day, &month, &year);

    struct Date expiry = calculateExpiryDate(day, month, year);
    printf("Expiration Date: %02d-%02d-%d\n", expiry.day, expiry.month, expiry.year);

    printf("Enter blood unit in ml (450 or 500): ");
    scanf("%d", &unit);

    FILE *add = fopen(bloodInventoryFiles[index], "a+");
    if (add == NULL)
    {
        printf("Error opening inventory file for %s!\n", bloodGroups[index]);
        return;
    }

    fprintf(add, "Id: %d, Date: %02d-%02d-%d, Unit: %d ml, Expiration: %02d-%02d-%d\n",
            p.id, day, month, year, unit, expiry.day, expiry.month, expiry.year);
    fclose(add);

    printf("Blood unit for donor added to inventory successfully.\n");
}

void displayAllDonors()
{
    FILE *f;
    struct Person p;
    f = fopen("D:\\project_blood_management\\Blood_donation\\donors.txt", "r");
    if (f == NULL)
    {
        printf("No donors found.\n");
        return;
    }

    printf("\nList of Donors:\n");
    while (fscanf(f, "%d,%[^,],%d,%[^,],%[^\n]\n", &p.id, p.name, &p.age, p.bloodGroup, p.phone) != EOF)
    {
        printf("ID: %d, Name: %s, Age: %d, Blood Group: %s, Phone: %s\n",
               p.id, p.name, p.age, p.bloodGroup, p.phone);
    }
    fclose(f);
}

void registerNewRecipient()
{
    int choice;
    printf("\n1.A+ 2.A- 3.B+ 4.B- 5.AB+ 6.AB- 7.O+ 8.O-\n");
    printf("Select Blood Group for Recipient (1-8): ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > 8)
    {
        printf("Invalid blood group selection.\n");
        return;
    }

    if (!isBloodAvailableForGroup(choice))
    {
        printf("Sorry, no units available. Cannot register recipient.\n");
        return;
    }

    FILE *f;
    struct Person p;
    f = fopen("D:\\project_blood_management\\Blood_donation\\recipients.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Recipient ID: ");
    scanf("%d", &p.id);
    getchar();

    printf("Enter Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &p.age);
    getchar();

    printf("Enter Blood Group: ");
    fgets(p.bloodGroup, sizeof(p.bloodGroup), stdin);
    p.bloodGroup[strcspn(p.bloodGroup, "\n")] = '\0';

    printf("Enter Phone: ");
    fgets(p.phone, sizeof(p.phone), stdin);
    p.phone[strcspn(p.phone, "\n")] = '\0';

    fprintf(f, "%d,%s,%d,%s,%s\n", p.id, p.name, p.age, p.bloodGroup, p.phone);
    fclose(f);
    printf("Recipient registered successfully.\n");
}

void displayAllRecipients()
{
    FILE *f;
    struct Person p;
    f = fopen("D:\\project_blood_management\\Blood_donation\\recipients.txt", "r");
    if (f == NULL)
    {
        printf("No recipients found.\n");
        return;
    }

    printf("\nList of Recipients:\n");
    while (fscanf(f, "%d,%[^,],%d,%[^,],%[^\n]\n", &p.id, p.name, &p.age, p.bloodGroup, p.phone) != EOF)
    {
        printf("ID: %d, Name: %s, Age: %d, Blood Group Taken: %s, Phone: %s\n",
               p.id, p.name, p.age, p.bloodGroup, p.phone);
    }
    fclose(f);
}

void findDonorByBloodGroup()
{
    FILE *f;
    struct Person p;
    char bg[10];
    int found = 0;

    f = fopen("D:\\project_blood_management\\Blood_donation\\donors.txt", "r");
    if (f == NULL)
    {
        printf("No donors found.\n");
        return;
    }

    printf("Enter Blood Group to search: ");
    scanf("%s", bg);

    while (fscanf(f, "%d,%[^,],%d,%[^,],%[^\n]\n",
                  &p.id, p.name, &p.age, p.bloodGroup, p.phone) != EOF)
    {
        if (strcmp(p.bloodGroup, bg) == 0)
        {
            printf("ID: %d, Name: %s, Age: %d, Phone: %s\n",
                   p.id, p.name, p.age, p.phone);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No donor found with blood group %s.\n", bg);
    }
    fclose(f);
}

void updateDonorDetails()
{
    FILE *f, *temp;
    struct Person p;
    int id, found = 0;

    printf("Enter Donor ID to update: ");
    scanf("%d", &id);
    getchar();

    f = fopen("D:\\project_blood_management\\Blood_donation\\donors.txt", "r");
    temp = fopen("D:\\project_blood_management\\Blood_donation\\temp.txt", "w");

    if (f == NULL || temp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    while (fscanf(f, "%d,%[^,],%d,%[^,],%[^\n]\n",
                  &p.id, p.name, &p.age, p.bloodGroup, p.phone) != EOF)
    {
        if (p.id == id)
        {
            printf("Enter new Name: ");
            fgets(p.name, sizeof(p.name), stdin);
            p.name[strcspn(p.name, "\n")] = '\0';

            printf("Enter new Age: ");
            scanf("%d", &p.age);
            getchar();

            printf("Enter new Blood Group: ");
            fgets(p.bloodGroup, sizeof(p.bloodGroup), stdin);
            p.bloodGroup[strcspn(p.bloodGroup, "\n")] = '\0';

            printf("Enter new Phone: ");
            fgets(p.phone, sizeof(p.phone), stdin);
            p.phone[strcspn(p.phone, "\n")] = '\0';

            found = 1;
        }
        fprintf(temp, "%d,%s,%d,%s,%s\n",
                p.id, p.name, p.age, p.bloodGroup, p.phone);
    }

    fclose(f);
    fclose(temp);

    remove("D:\\project_blood_management\\Blood_donation\\donors.txt");
    rename("D:\\project_blood_management\\Blood_donation\\temp.txt",
           "D:\\project_blood_management\\Blood_donation\\donors.txt");

    if (found)
        printf("Donor updated successfully.\n");
    else
        printf("Donor with ID %d not found.\n", id);
}

void registerHospital();
void showHospitalRecords();
void assignBloodToHospital();
void showBloodAssignments();

struct Hospital
{
    int id;
    char name[50];
    char address[100];
};

struct BloodAssignment
{
    int id;
    int blood_units;
    char date[20];
    char blood_group[5];
};

void registerHospital()
{
    struct Hospital h;
    FILE *fp;

    fp = fopen("D:\\project_blood_management\\hospital_reg\\hospital_name.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Hospital ID: ");
    scanf("%d", &h.id);
    getchar();

    printf("Enter Hospital Name: ");
    fgets(h.name, sizeof(h.name), stdin);

    printf("Enter Hospital Address: ");
    fgets(h.address, sizeof(h.address), stdin);

    fprintf(fp, "ID: %d, Name: %s, Address: %s\n", h.id, h.name, h.address);
    fclose(fp);

    printf("\nHospital registered successfully!\n");
}

void showHospitalRecords()
{
    FILE *fp;
    char ch;

    fp = fopen("D:\\project_blood_management\\hospital_reg\\hospital_name.txt", "r");
    if (fp == NULL)
    {
        printf("No hospital records found.\n");
        return;
    }

    printf("\n--- Registered Hospitals ---\n");
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);
}

void assignBloodToHospital()
{
    struct BloodAssignment b;
    FILE *fp;
    char buffer[300], tempFile[] = "D:\\project_blood_management\\hospital_reg\\temp_inventory.txt";
    int selectedId, found = 0;
    int choice;
    const char *bloodGroups[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};

    printf("\n=== BLOOD UNIT ASSIGNMENT TO HOSPITAL ===\n");
    printf("1.A+  2.A-  3.B+  4.B-  5.AB+  6.AB-  7.O+  8.O-\n");
    printf("Select Blood Group to assign (1-8): ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > 8)
    {
        printf("Invalid blood group selection.\n");
        return;
    }

    if (!isBloodAvailableForGroup(choice))
    {
        printf("Sorry, no units available for %s. Cannot assign to hospital.\n",
               bloodInventoryFiles[choice - 1]);
        return;
    }

    printf("\nAvailable Blood Units for %s:\n", bloodInventoryFiles[choice - 1]);
    FILE *show = fopen(bloodInventoryFiles[choice - 1], "r");
    if (show == NULL)
    {
        printf("Error opening inventory file.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), show))
    {
        printf("%s", buffer);
    }
    fclose(show);

    printf("\nEnter the ID of the blood unit to assign to hospital: ");
    scanf("%d", &selectedId);
    getchar();

    FILE *original = fopen(bloodInventoryFiles[choice - 1], "r");
    FILE *temp = fopen(tempFile, "w");

    if (original == NULL || temp == NULL)
    {
        printf("Error processing inventory file.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), original))
    {
        int currentId;
        if (sscanf(buffer, "Id: %d,", &currentId) == 1 && currentId == selectedId)
        {
            found = 1;
            continue;
        }
        fputs(buffer, temp);
    }

    fclose(original);
    fclose(temp);

    if (!found)
    {
        printf("Blood unit with ID %d not found. Cannot proceed.\n", selectedId);
        remove(tempFile);
        return;
    }

    remove(bloodInventoryFiles[choice - 1]);
    rename(tempFile, bloodInventoryFiles[choice - 1]);

    fp = fopen("D:\\project_blood_management\\hospital_reg\\Blood_unit_ass.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening assignment file!\n");
        return;
    }

    printf("Enter Hospital ID: ");
    scanf("%d", &b.id);

    printf("Enter Blood Units in ml: ");
    scanf("%d", &b.blood_units);

    printf("Enter Date (DD-MM-YYYY): ");
    scanf("%s", b.date);

    strcpy(b.blood_group, bloodGroups[choice - 1]);

    fprintf(fp, "ID: %d,Blood Unit: %d ml,Date: %s,Blood Group: %s,Assigned Unit ID: %d\n",
            b.id, b.blood_units, b.date, b.blood_group, selectedId);

    fclose(fp);
    printf("\nBlood unit ID %d assigned to Hospital ID %d successfully!\n", selectedId, b.id);
}

void showBloodAssignments()
{
    FILE *fp;
    char ch;

    fp = fopen("D:\\project_blood_management\\hospital_reg\\Blood_unit_ass.txt", "r");
    if (fp == NULL)
    {
        printf("No blood assignment records found.\n");
        return;
    }

    printf("\n--- Blood Unit Assignments ---\n");
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);
}

void hospitalManagementMenu()
{
    int choice;
    while (1)
    {
        printf("\n======== HOSPITAL MANAGEMENT SYSTEM =======\n");
        printf("\n1. Register Hospital\n");
        printf("2. Show Hospital Records\n");
        printf("3. Assign Blood Units\n");
        printf("4. Show Blood Assignments\n");
        printf("5. Return to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            registerHospital();
            break;
        case 2:
            showHospitalRecords();
            break;
        case 3:
            assignBloodToHospital();
            break;
        case 4:
            showBloodAssignments();
            break;
        case 5:
            return;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
}

void bloodDonationMenu()
{
    int choice;
    while (1)
    {
        printf("\n======= BLOOD DONATION MANAGEMENT SYSTEM =======\n");
        printf("\n1. Register Donor\n");
        printf("2. Display Donors\n");
        printf("3. Update Donor\n");
        printf("4. Register Recipient\n");
        printf("5. Display Recipients\n");
        printf("6. Search Donor By Blood Group\n");
        printf("7. Return to Main Menu\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            registerNewDonor();
            break;
        case 2:
            displayAllDonors();
            break;
        case 3:
            updateDonorDetails();
            break;
        case 4:
            registerNewRecipient();
            break;
        case 5:
            displayAllRecipients();
            break;
        case 6:
            findDonorByBloodGroup();
            break;
        case 7:
            return;
        default:
            printf("Invalid Choice. Try again.\n");
        }
    }
}

void bloodInventoryMenu()
{
    int choice;
    while (1)
    {
        printf("\n======== BLOOD INVENTORY MANAGEMENT SYSTEM ========\n");
        printf("\n1. Manual Inventory Adjustment\n");
        printf("2. Display Blood Unit\n");
        printf("3. Display Full Blood Units\n");
        printf("4. Return to Main Menu\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            adjustInventoryManually();
            break;
        case 2:
            displaySingleBloodGroupInventory();
            break;
        case 3:
            displayFullInventoryChart();
            break;
        case 4:
            return;
        default:
            printf("Invalid Choice. Try again.\n");
        }
    }
}

int main()
{
    int mainChoice;

    while (1)
    {
        printf("\n================  Welcome To Blood Management System    ==============\n");
        printf("\n1. Blood Donor Registration\n");
        printf("2. Blood Inventory\n");
        printf("3. Hospital Management\n");
        printf("4. Exit System\n");
        printf("Enter Your Choice: ");
        scanf("%d", &mainChoice);

        switch (mainChoice)
        {
        case 1:
            bloodDonationMenu();
            break;
        case 2:
            bloodInventoryMenu();
            break;
        case 3:
            hospitalManagementMenu();
            break;
        case 4:
            printf("Exiting system. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
