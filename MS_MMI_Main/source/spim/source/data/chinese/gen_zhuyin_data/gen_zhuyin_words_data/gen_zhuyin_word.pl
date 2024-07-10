#ี๛ภํดสื้
#[in]
#        zhuyin_words.txt
#[out]
#        word_1.txt
#        word_2.txt
#        word_3.txt
#        word_4.txt
#        word_5.txt
#        word_6.txt
#        report.txt



open(IN_FILE, "< :raw", "zhuyin_words.txt");

open($file_hash{1}, ">word_1.txt") or die "can not create file!";
open($file_hash{2}, ">word_2.txt") or die "can not create file!";
open($file_hash{3}, ">word_3.txt") or die "can not create file!";
open($file_hash{4}, ">word_4.txt") or die "can not create file!";
open($file_hash{5}, ">word_5.txt") or die "can not create file!";
open($file_hash{6}, ">word_6.txt") or die "can not create file!";
open($file_hash{7}, ">report.txt") or die "can not create file!";

die "Not valide File." unless (read(IN_FILE, $buf, 2) == 2);

$stop_decode = 0;

while(read(IN_FILE, $buf, 10240)) {
	my @uni_codes = unpack("S*", $buf);

	foreach $uni (@uni_codes) {
	    if ($uni == 0x20) {
	        $stop_decode = 1;
	    }
	    elsif ($uni == 0xa) {
	        my $string = join ", ", @data;

	        $count = @data;
	        die "overflow!" if $count > 6;
	        print {$file_hash{$count}} $string, "\n";
	        
	        $count_hash{$count}++;
	        
	        @data = ();
	        $stop_decode = 0;
	    }
	    elsif ($stop_decode == 0) {
	        my $string = sprintf "0x%x", $uni;
	        push @data, $string;
    	}
	}
}

@count_key = keys %count_hash;
@count_key = sort @count_key;

foreach $count (@count_key) {
    print {$file_hash{7}} "$count  =>  $count_hash{$count}\n";
}

foreach $i (1..7) {
    close $file_hash{$i};
}
