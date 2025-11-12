bool isPalindrome(int x) {

    if (x < 0)
        return false;

    int reversed = 0;
    while(x > 0)
    {
        reversed = reversed * 10 + x % 10;
        x /= 10;
    }

    return x == reversed;
}




main()
{
    isPalindrome(121);
    return 0;
}